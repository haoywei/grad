#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ether.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#define max_size 1518

unsigned short ComputeChecksum(unsigned char *addr, int len)
{
  int range = 0;
  long sum = 0;
  unsigned char *current = addr;
  unsigned short result = 0;

  while (range < len) {
    sum += *current++;
    range += 2;
  }

  if (range == len) {
    *(unsigned char *) (&result) = *(unsigned char *) current;
    sum += result;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return (result);
}


void callback(u_char *user, const struct pcap_pkthdr *h , const u_char *packet){
	int i ;
	struct sockaddr_ll socket_addr = {0};
	struct ether_header *e1 = (struct ether_header*) packet;
	struct iphdr *ip1 = (struct iphdr*) (packet + ETH_HLEN);
	struct icmphdr *icmp1 = (struct icmphdr*) (packet + ETH_HLEN + (ip1->ihl)*4);
	struct sockaddr_in sock_addr;
	u_char *returnp;
	returnp = (u_char *) malloc(h->len);
	sock_addr.sin_addr.s_addr = ip1->saddr;
    	printf("!! %s -> " ,inet_ntoa(sock_addr.sin_addr));
    	sock_addr.sin_addr.s_addr = ip1->daddr;
    	printf("%s. Len %d.---- ICMP: type %d----- code %d\n", 
        inet_ntoa(sock_addr.sin_addr), h->len, icmp1->type, icmp1->code);	

	struct ifreq ifidx = {0};

	if( icmp1->type == ICMP_ECHO && icmp1->code == 0){
		int swap;
		for( i=0; i<6;++i){
			swap = e1->ether_shost[i];
			e1->ether_shost[i] = e1->ether_dhost[i];
			e1->ether_dhost[i] = swap;
		}
		swap = ip1->daddr;
		ip1->daddr = ip1->saddr;
		ip1->saddr = swap;
		icmp1->type = 0;
		icmp1->checksum = 0;
		icmp1->checksum = ComputeChecksum((unsigned char*)icmp1, h->len-14-4*(ip1->ihl)) ;
		memcpy(returnp,e1,ETH_HLEN);
	 	memcpy(returnp +ETH_HLEN ,ip1 , 4*(ip1->ihl));
		memcpy(returnp+ETH_HLEN+20,icmp1,h->len-ETH_HLEN-4*(ip1->ihl));
		int sd;
	if ((sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP)))<0)
	 {
    		perror("raw socket");
    		exit(1);
 	 }
	strncpy(ifidx.ifr_name, "eth14", strlen("eth14"));
        if( ioctl(sd, SIOCGIFINDEX, &ifidx) < 0 ) {
                perror("Interface index error");
                exit(1);
        }
  	for ( i = 0; i< 6 ;++i){
       		socket_addr.sll_addr[i] = e1->ether_dhost[i];
        }
  	socket_addr.sll_halen = ETH_ALEN;
  	socket_addr.sll_ifindex = ifidx.ifr_ifindex;
 	 if((sendto(sd, returnp, h->len, 0, (struct sockaddr*)&socket_addr, sizeof(struct sockaddr_ll))) < 0) {
        	perror(" Cannot send spoofed frame");
        	exit(1);
  	}
  	else{
       		 printf("Send frame sucessfully!\n");}
	

}}






int main( int argc, char *argv[] )
{	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t * sniff;
	struct bpf_program fp; 
	if((sniff = pcap_open_live("eth14", max_size, 1, 100, errbuf)) == NULL ) {  
        	perror("Cannot open deveice");
        	exit(1);
    	} 

	if( pcap_compile(sniff, &fp, "icmp", 0, 0) == -1 )   {
        	perror("Cannot Compile");
        	exit(1);
    	}
	
	if( pcap_setfilter(sniff, &fp) == -1 ) {
        	perror("Set filter error");
		exit(1);
    	}
	pcap_loop(sniff, 100 , callback , NULL);
	
	
	pcap_freecode(&fp);                              
    	pcap_close(sniff);
	printf("Sniffing complete\n" );
	return 0;                                   

}
