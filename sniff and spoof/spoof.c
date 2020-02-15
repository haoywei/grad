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




#define dsize 32

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



struct iphdr* CreateIpHeader()
{
	struct iphdr* ip_header;
	ip_header = (struct iphdr* )malloc(sizeof(struct iphdr));

	int iphdr_len = sizeof(struct iphdr) +  sizeof(struct icmphdr) + dsize;
	ip_header->version = 4;
	ip_header->ihl = sizeof(struct iphdr) / 4 ;
	ip_header->tos = 0;
	ip_header->tot_len = htons(iphdr_len);
	ip_header->id = htons(1234);
	ip_header->frag_off = 0;
	ip_header->ttl = 64;
	ip_header->protocol = IPPROTO_ICMP;
	ip_header->check = 0; 
	ip_header->saddr = inet_addr("192.168.15.4");
	ip_header->daddr = inet_addr("192.168.15.5");


	ip_header->check = ComputeChecksum((unsigned char *)ip_header, sizeof(struct iphdr));

	return (ip_header);

}

struct icmphdr* CreateIcmpHeader() {
	struct icmphdr* icmp_header = (struct icmphdr*)malloc( sizeof(struct icmphdr) );
	icmp_header->code = 0;
	icmp_header->type = ICMP_ECHO;
	icmp_header->checksum = 0;
	icmp_header->checksum = ComputeChecksum((unsigned char *)icmp_header, sizeof(struct icmphdr) );
	return (icmp_header);
}

int main(int argc, char **argv)
{
		int sd,on = 1;
		struct sockaddr_in sin = {                    
            	.sin_zero        = { 0,0,0,0,0,0,0,0 },    
           	.sin_family      = AF_INET,                 
           	.sin_port        = 0,                        
            	.sin_addr.s_addr = inet_addr("192.168.15.5")           
        	};
		sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
		if(sd < 0) {
			perror("socket() error"); 
			exit(-1);
			}
		if( setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0 ) {
       			 perror("[-] Error! Cannot set IP_HDRINCL");
        		return -1;
    		}

		unsigned char* packet;
		struct icmphdr* icmp;
		struct iphdr* ip;
		char* data;
		ip = CreateIpHeader();
		icmp = CreateIcmpHeader();
		int plen = ntohs(ip->tot_len);
		packet = (unsigned char *)malloc(plen);
		data = (unsigned char *)malloc(dsize);
		int i;
		for( i = 0; i < dsize ; i++){
			data[i] = 0 ;

		}
		memcpy(packet, ip, sizeof(struct iphdr));
		memcpy((packet + sizeof(struct iphdr)), icmp, sizeof(struct icmphdr) );
		memcpy((packet + sizeof(struct iphdr) + sizeof(struct icmphdr)), data, dsize);
		
		if(sendto(sd, packet, plen, 0, (struct sockaddr*)&sin, sizeof(sin)) < 0){
       			 perror("[-] Error! Cannot send spoofed packet");
        		return -1;
    		}
    		else 
        		printf( "[+] Spoofed IP packet sent successfully!\n");	
		
		return 0;




	



}
