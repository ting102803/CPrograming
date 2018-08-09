#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT_NUM 5650
#define BUFSIZE 1024
struct cal_data {
	int left_num;
	int right_num;
	char op;
	int result;
	short int error;
};//입력받은 숫자와 연산자에서 계산하기위해 저장하는 구조체
int main(int argc, char **argv) {
	int sockfd;//소켓연결이 잘되어있는지 확인하기위한 지정번호
	socklen_t addrlen; //주소의 사이즈를 저장
	int cal_result; //계산결과를 저장
	int left_num, right_num; // 입력받은 피연산자 2개
	struct sockaddr_in addr, cliaddr; // sockaddr_in은 IPv4주소체계를 사용하는 소켓구조체이다
	struct cal_data Inputdata;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("socket error\n");
		return 1;
	}//UDP 소켓지정이 잘못되어 에러가 발생하고 프로그램이 종료한다
	memset((void *)&addr, 0x00, sizeof(addr));//addr값을 0으로 초기화
	addr.sin_family = AF_INET;//IPv4 주소체계에서 사용항상 AF_INET으로 설정한다
	addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY는 서버의 IP주소를 자동으로 찾아서 대입해주는 함수로서 서버의 주소를 소켓의 주소에 저장한다
	addr.sin_port = htons(PORT_NUM);//포트번호를 소켓의포트에 저장한다
	addrlen = sizeof(addr);
	if(bind(sockfd, (struct sockaddr *)&addr, addrlen) == -1) {
		printf("bind error\n");
		return 1;
	}//bind가 잘못되어 에러가 발생하고 프로그램이 종료한다
	printf("Start to run server!\n");
	while(1) {
		addrlen = sizeof(cliaddr);
		recvfrom(sockfd, (void *)&Inputdata, sizeof(Inputdata), 0, (struct sockaddr *)&cliaddr,&addrlen);
		//클라이언트의 메세지를 받는데 Inputdata에 해당크기만큼 저장한다
		printf("Client Info : %s (%d)\n", inet_ntoa(cliaddr.sin_addr), ntohs (cliaddr.sin_port));
		printf("Input : %d %c %d\n", ntohl(Inputdata.left_num), Inputdata.op, ntohl(Inputdata.right_num));
		//읽어드린 정보를 출력한다
		left_num = ntohl(Inputdata.left_num);
		right_num = ntohl(Inputdata.right_num);//네트워크바이트 순서를 호스트 바이트 순서로 바꾼다
		switch(Inputdata.op) {// 클라이언트에서부터 받은 연산에따라 연산을 하는 스위치문
			case '+':
			cal_result = left_num + right_num;
			break;
			case '-':
				cal_result = left_num - right_num;
			break;
			case '*':
				cal_result = left_num * right_num;
			break;
			case '/':
				if(right_num == 0){//0으로 나눴을때 에러잡기용
				printf("right num is zero!!\n");
				cal_result = 0;
				Inputdata.error = htons(2);
			break;}
				cal_result = left_num / right_num;//0이아니라면 나눗셈 진행
			break;
			default:
				Inputdata.error = htons(2);
				cal_result = 0;
				printf("operator error\n");
		}
		printf("Result : %d\n ",cal_result);
		Inputdata.result=htonl(cal_result);//호스트 바이트순서를 네트워크바이트순서로 바꾼다
		sendto(sockfd, (void *)&Inputdata, sizeof(Inputdata), 0,(struct sockaddr *)&cliaddr, addrlen);
	}//클라이언트에게 계산된값을 보낸다.
return 1;
}
