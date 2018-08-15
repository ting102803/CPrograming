#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#define PORTNUM 5650
#define MAXLINE 1024
#define Q_UPLOAD 1
#define Q_DOWNLOAD 2
#define Q_LIST 3
struct Cquery
{
	int command;
	char fname[64];
};
	void help(char *progname)
{
	printf("Usage : %s -h -i [ip] // -u [upload filename] // -d [download filename] // -l\n",progname);
}
int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	int sockfd;
	int clilen;
	int opt;
	int optflag=0;
	char ipaddr[36]={0x00,};
	int command_type=0;
	char fname[80]={0x00,};
	char buf[MAXLINE];
	while( (opt = getopt(argc, argv, "hli:u:d:")) != -1)
	{//입력들어온 알맞은 값에 따라 함수가 실행된다
		switch(opt)
		{
			case 'h':
				help(argv[0]);
			return 1;
			case 'i':
				sprintf(ipaddr, "%s", optarg);
			break;
			case 'u':
				command_type = Q_UPLOAD;
				sprintf(fname, "%s", optarg);
				optflag = 1;
			break;
			case 'd':
				command_type = Q_DOWNLOAD;
				sprintf(fname, "%s", optarg);
				optflag = 1;
			break;
			case 'l':
				command_type = Q_LIST;
			break;
			default:
				help(argv[0]);
			return 1;
		}
	}
	if(ipaddr[0] == '\0') {
		printf ("ip address not setting\n");
		return 0;
	}
	if((fname[0] == '\0') && (optflag == 1)) {
		printf ("fname error\n");
		return 0;
	}
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		printf("socket error");
		return 1;
	}
	memset((void *)&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipaddr);
	addr.sin_port = htons(PORTNUM);
	clilen = sizeof(addr);
	if(connect(sockfd, (struct sockaddr *)&addr, clilen) < 0)
	{
		printf("connect error");
		return 0;
	}
	while(1)
	{//입력에 맞게 실행
		switch(command_type)
		{
			case (Q_LIST):
				get_list(sockfd);
			break;
			break;
			case (Q_DOWNLOAD):
				download(sockfd, fname);
			break;
			case (Q_UPLOAD):
				upload(sockfd, fname);
			break;
			default:
				printf("unknown command\n");
			break;
		}
		break;
	}
	close(sockfd);
}
int download(int sockfd, char *file)
{//다운로드를 요청할경우
	struct Cquery query;
	int fd;
	int readn, writen;
	char buf[MAXLINE];
	if( (fd = open(file, O_WRONLY|O_CREAT|O_TRUNC)) == -1 )
	{//파일이 없다면 생성하고 있더라면 덮어쓰기를 하여 파일을 생성한다.
		printf("file make error");
		return -1;
	}
	memset(buf, 0x00, MAXLINE);
	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_DOWNLOAD);
	sprintf(query.fname, "%s", file);//sprintf로 char배열로 옮겨준다
	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0)
	{//커리 요청 메세지를 보낸다
		return -1;
	}
	while((readn = recv(sockfd, buf, MAXLINE, 0)) > 0)
	{
		writen = write(fd, buf, readn);//서버로부터 받은 버퍼를 파일에 작성한다.
		if(writen != readn)
		{
			return -1;
		}
		memset(buf, 0x00, MAXLINE);
	}
	close(fd);
	return 1;
}
int get_list(int sockfd)
{
	struct Cquery query;
	char buf[MAXLINE];
	int len;
	int readn;
	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_LIST);
	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0 )
	{//list요청 메세지를 보냄
		printf("List Send Error\n");
		return -1;
	}
	memset(buf, 0x00, MAXLINE);	
	while(1)
	{
		len = recv(sockfd, buf, MAXLINE, 0);
		if(len <= 0) break;
		printf("%s", buf);
		memset(buf, 0x00, MAXLINE);
	}//받은 버퍼를 바로 출력해준다.
	printf("End!\n");
}
int upload(int sockfd, char *file)
{
	struct Cquery query;
	int fd;
	int readn;
	int sendn;
	char buf[MAXLINE];
	if( (fd = open(file, O_RDONLY)) == -1 ) {
		return -1;
	}//파일명을 읽고 디스크립퍼를 저장한다
	memset(&query, 0x00, sizeof(query));
	query.command = htonl(Q_UPLOAD);
	sprintf(query.fname, "%s", file);
	if(send(sockfd, (void *)&query, sizeof(query), 0) <=0) {
	return -1;
	}
	while((readn = read(fd, buf, MAXLINE)) > 0)
	{//파일을 읽을게 없을때 까지 읽고
		sendn = send(sockfd, buf, readn, 0);//읽은 버퍼를 보내준다.
		if(sendn != readn)
		{
			printf("Upload Error\n");
		return -1;
		}
	}
	close(fd);
	return 1;
}
