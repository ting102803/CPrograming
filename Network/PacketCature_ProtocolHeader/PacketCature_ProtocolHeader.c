#include<netinet/in.h>
#include<stdio.h> 
#include<netinet/ip_icmp.h>   
#include<netinet/udp.h> 
#include<netinet/tcp.h> 
#include<netinet/ip.h>   
#include<netinet/if_ether.h>  
#include<net/ethernet.h>  
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/types.h>

#include<stdlib.h>    
#include<string.h>    
#include<unistd.h>

#define PACKET_LENGTH 65536

void PrintPacket(unsigned char* , int);
void ip_header(unsigned char* , int);
void tcp_packet(unsigned char * , int );
void udp_packet(unsigned char * , int );
void icmp_packet(unsigned char* , int );
void PrintData (unsigned char* , int);

struct sockaddr_in source,dest;
 
int main()
{
    int readn;
    socklen_t addrlen;
    int sock_raw;
    struct sockaddr_in saddr;
         
    unsigned char *buffer = (unsigned char *) malloc(PACKET_LENGTH); //버퍼 초기화
     
    printf("Start!\n");
     
    sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL));//모든프로토콜 받는 소켓 선언
     
    if(sock_raw < 0)
    {
        printf("socket error");
        return 1;
    }
    while(1)
    {
        addrlen = sizeof(saddr);
		memset(buffer,0x00,PACKET_LENGTH);//시작할때마다 버퍼를 깨끗하게한다
        readn = recvfrom(sock_raw , buffer , PACKET_LENGTH , 0 , (struct sockaddr *)&saddr , (socklen_t*)&addrlen);
        //소켓으로 패킷을 받는다.
		if(readn <0 )
        {
            printf("readn error\n");
            return 1;
        }
		
        PrintPacket(buffer , readn);
    }
    close(sock_raw);
    printf("End");
    return 0;
}
 
void PrintPacket(unsigned char* buffer, int size)
{
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    switch (iph->protocol) 
    {
        case 1:  
            icmp_packet(buffer , size);//Icmp는 프로토콜번호가 1번으로 핑을 보내보면알수있다.
            break;
        case 2: 
            break;
         //2번은 IGMP이다.
        case 6:  
            tcp_packet(buffer , size);//TCP는 프로토콜번호가 6번으로 현재 인터넷통신을할때 주로쓰인다.
            break;
         
        case 17: 
            udp_packet(buffer , size);//UDP는 17번이다.
            break;
         
        default:
            break;
    }
   
}
 
void ethernet_header(unsigned char* Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    printf( "\n");
    printf( "Ethernet Header\n");
    printf( "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    printf( "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    printf( "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}//이더넷 헤더에는 도착주소 출발주소 그리고 프로토콜이 저장되어있다.
 
void ip_header(unsigned char* Buffer, int Size)
{
    ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf( "\n");
    printf( "IP Header\n");
    printf( "   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf( "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf( "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf( "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf( "   |-Identification    : %d\n",ntohs(iph->id));
    printf( "   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf( "   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf( "   |-Checksum : %d\n",ntohs(iph->check));
    printf( "   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
    printf( "   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
}//IP헤더에는 버전,헤더길이,IP길이,TTL,프로토콜,체크섬등이 있다.
 
void tcp_packet(unsigned char* Buffer, int Size)
{//프로토콜이 6번일때 TCP이다 TCP는 다양한 기능들이 있다. 그만큼 헤더의 양도 많다.
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    printf( "\n\n------------------TCP Packet---------------------\n");  
         
    ip_header(Buffer,Size);
         
    printf( "\n");
    printf( "TCP Header\n");
    printf( "   |-Source Port      : %u\n",ntohs(tcph->source));
    printf( "   |-Destination Port : %u\n",ntohs(tcph->dest));
    printf( "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    printf( "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    printf( "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    printf( "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    printf( "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    printf( "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    printf( "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    printf( "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    printf( "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    printf( "   |-Window         : %d\n",ntohs(tcph->window));
    printf( "   |-Checksum       : %d\n",ntohs(tcph->check));
    printf( "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    printf( "\n");
    printf( "\n");               
    printf( "\n---------------------------------------------------------------");
}
 
void udp_packet(unsigned char *Buffer , int Size)
{
     //프로토콜이 17번일때 UDP이다 UDP는 단순하고 기능이 적은대신 빠른 통신 방법이기때문에 헤더의 크기는 작다.
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    printf( "\n\n--------------------UDP Packet--------------------\n");
     
    ip_header(Buffer,Size);           
     
    printf( "\nUDP Header\n");
    printf( "   |-Source Port      : %d\n" , ntohs(udph->source));
    printf( "   |-Destination Port : %d\n" , ntohs(udph->dest));
    printf( "   |-UDP Length       : %d\n" , ntohs(udph->len));
    printf( "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    printf( "\n"); 
    printf( "\n------------------------------------------------------");
}
 
void icmp_packet(unsigned char* Buffer , int Size)
{//Icmp는 대표적으로 Ping보내 서로 연결이 잘되어있는지 확인할때 사용되는 프로토콜이다.
//간단한 기능인 만큼 헤더의 크기는 작은편이다.
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
     
    printf( "\n\n--------------------ICMP Packet--------------------\n"); 
     
    ip_header(Buffer , Size);
             
    printf( "\n");
         
    printf( "ICMP Header\n");
    printf( "   |-Type : %d",(unsigned int)(icmph->type));
    printf( "   |-Code : %d\n",(unsigned int)(icmph->code));
    printf( "   |-Checksum : %d\n",ntohs(icmph->checksum));

    printf( "\n");
    printf( "\n------------------------------------------------------------");
}
 

