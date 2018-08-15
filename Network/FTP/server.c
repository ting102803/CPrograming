#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#define PORTNUM 5650
#define MAXLINE 1024
#define Q_UPLOAD 1
#define Q_DOWNLOAD 2
#define Q_LIST 3
#define LOWER_BOUNDER "========================================="
struct Cquery
{
	int command;
	char f_name[64];
};
int process(int sockfd);
int file_upload(int sockfd, char *filename);
int file_list(int sockfd);
int file_download(int sockfd, char *filename);
int whattime(char *tstr);
int fline_cnt( char *name );//list count를 알아내기위한 함수

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	int sockfd, cli_sockfd;
	int clilen;
	int pid;
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return 1;
	}
	printf("Start Server\n");
	memset(&addr, 0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORTNUM);
	if( bind (sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
		printf("bind error");
		return 1;
	}
	if( listen(sockfd, 5) != 0)
	{
		printf("listen error");
		return 1;
	}
	signal(SIGCHLD, SIG_IGN);
	while(1)
	{
		clilen = sizeof(addr);
		cli_sockfd = accept(sockfd, NULL, &clilen);
		if(cli_sockfd < 0) exit(0);
		pid = fork();
		if(pid == 0)
		{
			process(cli_sockfd);
			printf("close socket\n");
			sleep(5);
			close(cli_sockfd);
			exit(0);
		}
		else if(pid == -1)
		{
			printf("fork error\n");
			exit(0);
		}
		else
		{
			close(cli_sockfd);
		}
	}
	}

int process(int sockfd)
{
	char buf[MAXLINE];
	struct Cquery query;
	while(1)
	{
		if(recv(sockfd, (void *)&query, sizeof(query), 0) <= 0)
		{
			return -1;
		}
		printf("read ok\n");
		query.command = ntohl(query.command);
		switch(query.command)
	{
	case (Q_UPLOAD):
		file_upload(sockfd, query.f_name);
	break;
	case (Q_DOWNLOAD):
		file_download(sockfd, query.f_name);
	break;
	case (Q_LIST):
		file_list(sockfd);
	break;//알맞은 명령에 맞게  함수를 실행시킨다.
	}
	break;
	}
	return 1;
	}
int file_upload(int sockfd, char *filename)
{
	int fd;//디스크립터 저장용 변수
	int readn;
	int writen;
	char buf[MAXLINE];
	struct sockaddr_in addr;
	int addrlen;
	char tstr[22];//현재시각을 리턴해서 char배열로 저장하기위한 배열
	char sizech[6];//파일사이즈 저장할 배열
	int size;//사이즈를 추출하고 저장할 변수
	int ret;
	FILE *fp;//사이즈를 추출할때 쓰는 FILE 변수
	int count;
	char countch[5];//현재의 리스트파일을 읽고 count를 확인
	if( (fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC)) == -1)
	{//읽어온 파일명이 없다면 생성하고 이미 있다면 지우고 다시 재생성한다.
		return -1;
	}
	memset(buf, 0x00, MAXLINE);
	while((readn = recv(sockfd, buf, MAXLINE, 0)) > 0)//다 읽을때까지 읽는다.
	{
		writen = write(fd, buf, readn);//읽은 양 만큼 작성한다
		if(writen < 0) break;
		memset(buf, 0x00, MAXLINE);
	}
	close(fd);//파일 업로드가 끝난후 파일을 닫는다
	fp = fopen(filename, "r");//size 측정을 위해 다시 fopen으로 연다
	fseek(fp, 0, SEEK_END); 
	size = ftell(fp); //fseek를 이용하여서 파일크기를 측정한다
	sprintf(sizech,"%d",size);//출력을 위해 char배열에 저장
	fclose(fp);//사용이 끝났음으로 파일을 닫는다
	
	ret = open("./list.txt", O_WRONLY|O_CREAT|O_APPEND);//리스트 파일이 없으면 새로열고 있으면 계쏙 추가하는 식이다.
	count=fline_cnt("./list.txt");//현재 리스트의 줄수를 세서 리턴
	count = count/5;//5줄마다 한 파일씩 작성되기때문에 5로 나누면 count 1개이다.
	sprintf(countch,"%d",count);//
	if(ret == -1)
	{
		return -1;
	}
	
	whattime(tstr);//현재시각을 리턴하여서 저장
	write(ret,"name = ",7);
	write(ret,filename,strlen(filename));//파일명을 입력
	write(ret,"\nip = ",6);//ip주소 입력
	write(ret,inet_ntoa(addr.sin_addr),strlen(inet_ntoa(addr.sin_addr)));	
	write(ret,"\nup_date = ",11);
	write(ret,tstr,strlen(tstr));
	write(ret,"\ncount = ",9);
	write(ret,countch,strlen(countch));
	write(ret,"\nsize = ",8);//앞서 저장한 시각 카운트 사이즈 입력
	write(ret,sizech,strlen(sizech));
	write(ret,"\n",1);//출력에 필요한 양식으로 list.txt에 출력을 한다
	
	addrlen = sizeof(addr);
	getpeername(sockfd, (struct sockaddr *)&addr, &addrlen);
	printf("File Upload %s\n", inet_ntoa(addr.sin_addr));
	return 1;
}

int file_download(int sockfd, char *filename)
{//다운로드 요청이왔을경우에는 서버의 입장에선 클라이언트로 보내야한다
	int fd;
	int readn;
	int sendn;
	char buf[MAXLINE];
	if( (fd = open(filename, O_RDONLY)) == -1)
	{
		printf("open error\n");
		return -1;
	}//원하는 파일이 있는지 확인후
	memset(buf, 0x00, MAXLINE);
	while( (readn = read(fd, buf, MAXLINE)) > 0)
	{
		sendn = send(sockfd, buf, readn, 0);
		if(sendn < 0) break;
		memset(buf, 0x00, MAXLINE);
	}//파일을 읽으면서 보낸다
	if (sendn == -1) {
	printf("send error\n");	
	return -1;}
	return 1;
}



int whattime(char *tstr)
{
	struct tm *tm_ptr;
	time_t the_time;
	time(&the_time);
	tm_ptr = localtime(&the_time);
	sprintf(tstr, "%d//%d//%d-%d:%d:%d",
	tm_ptr->tm_year + 1900,
	tm_ptr->tm_mon + 1,
	tm_ptr->tm_mday,
	tm_ptr->tm_hour,
	tm_ptr->tm_min,
	tm_ptr->tm_sec);
}

int file_list(int sockfd)
{//리스트를 요청할땐 list.txt에 있는 데이터를 클라이언트로 보내면된다.
	//다운로드와 같이 전송하는것은 같지만 클라이언트에서 파일로 저장할지 바로 출력할지 선택하면된다.
	char *ErrMsg;
	int ret;
	int readn;
	int sendn;
	char buf[MAXLINE];
	ret = open("./list.txt", O_RDONLY);
	if(ret == -1)
	{
		printf("Open Error\n");
		return -1;
	}
	printf("Open Success\n");
	while( 0 < ( readn = read( ret, buf, MAXLINE-1)))   //
      {     
		sendn = send(sockfd, buf, readn, 0);//읽으면서 보낸다.
		if(sendn < 0) break;
		memset(buf, 0x00, MAXLINE);
      }
	return 1;
}
int fline_cnt( char *name )//파일 라인수를 리턴하는 함수이다.
{ 
    FILE *fp; 
    char buf[MAXLINE];
    int i; 

    fp = fopen(name, "r" ); 
    for( i = 0, buf[MAXLINE-2] = 0; fgets( buf, MAXLINE, fp ); buf[MAXLINE-2] = 0 )
        ( buf[MAXLINE-2] && buf[MAXLINE-2] != '\n' )? : i++;
    fclose(fp); 
    return i; 
}

