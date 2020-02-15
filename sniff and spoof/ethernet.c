#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>
#define dsize 1500
int main(){
  int sd;

  struct sockaddr_ll socket_addr;
  
   
  
  struct ether_header *eh = malloc(sizeof(struct ether_header)+dsize);
  unsigned char src_mac[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
  unsigned char dst_mac[6] = {0x02, 0x04, 0x06, 0x08, 0x10, 0x12}; 
  
  eh->ether_type = htons(ETH_P_IP);  
  int i = 0;
  if ((sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP)))<0)
 {
    perror("raw socket");
    exit(1);
  }
  for ( i = 0; i< 6 ;i++){
	eh->ether_shost[i] = src_mac[i];
	eh->ether_dhost[i] = dst_mac[i];
	socket_addr.sll_addr[i] = dst_mac[i];

	}

  socket_addr.sll_halen = ETH_ALEN;
  socket_addr.sll_ifindex = 2;
  
  if((sendto(sd, eh, sizeof(struct ether_header)+ dsize , 0, (struct sockaddr*)&socket_addr, sizeof(struct sockaddr_ll))) < 0) {
        perror(" Cannot send spoofed frame");
        exit(1);
  }
  else{
	printf("Send frame sucessfully!\n");}


  return 0;
}
