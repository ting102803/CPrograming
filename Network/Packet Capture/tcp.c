#include <netinet/in.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PACKET_LENGTH 65536

void PrintPacket(unsigned char* buffer , int size);
void PrintTcp(unsigned char* buf , int size);
void PrintData (unsigned char* buffer,int size);

int main(int argc,char **argv)
{
	int readn;
	socklen_t addrlen;
	int sock_raw;
	struct sockaddr_in saddr;

	unsigned char *buffer = (unsigned char *)malloc(PACKET_LENGTH);

	sock_raw = socket(AF_INET, SOCK_RAW,IPPROTO_TCP);//TCP관련 출력이기때문에 TCP로 인자값을 해준다
	if(sock_raw<0)
	{
		return 1;
	}
	while(1)
	{
		addrlen = sizeof(saddr);
		memset(buffer,0x00,PACKET_LENGTH);
		readn = recvfrom(sock_raw,buffer,PACKET_LENGTH,0,(struct sockaddr *)&saddr,&addrlen);
		if(readn<0)
		{
			return 1;
		}
		PrintPacket(buffer,readn);
	}
	close(sock_raw);
	return 0;
}
void PrintPacket(unsigned char* buffer,int size){
	struct iphdr *iph = (struct iphdr*)buffer;
	printf("protocol : %d\n",iph->protocol);
	switch(iph->protocol)//IP hear에 있는 프로토콜숫자를 보고 출력
	{
		case 1:
			printf("ICMP");
			break;
		case 2:
			printf("IGMP");
			break;
		case 6:
			printf("TCP");
			PrintTcp(buffer,size);//6번일경우 TCP이기때문에 PrintTcp를 써서 출력
			break;
		case 17:
			printf("UDP");
			break;
		case 18:
			printf("MUX");
			break;
	default:
		break;
	}
}

void PrintTcp(unsigned char* buf,int size)
{
	unsigned short iphdrlen;
	unsigned char *data;

	struct iphdr *iph = (struct iphdr *)buf;
	iphdrlen = iph->ihl*4;
	struct tcphdr *tcph = (struct tcphdr*)(buf+iphdrlen);

	data = (unsigned char *) ( buf+ (iph->ihl*4) + (tcph->doff*4));
	printf("%s",data);
}
