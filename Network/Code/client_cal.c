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
	int sockfd;
	struct sockaddr_in addr;
	struct cal_data Inputdata, recvaddr;
	char msg[BUFSIZE];
	int left_num;
	int right_num;
	socklen_t addrlen;
	char op[2];
	if (argc != 2) {
		printf("Please input \" %s and [IPv4]\"\n", argv[0]);
		return 1;
	}//클라이언트가 실행시 인자를 2개를 못받을경우 즉실행과 IP주소를 못받을경우 에러 메세지 출력
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ) {
		return 1;
	}//UDP 소켓지정이 잘못되어 에러가 발생하고 프로그램이 종료한다
	memset((void *)&addr, 0x00, sizeof(addr));//addr값을 0으로 초기화
	addr.sin_family = AF_INET;//IPv4 주소체계에서 사용항상 AF_INET으로 설정한다
	addr.sin_addr.s_addr = inet_addr(argv[1]);//INADDR_ANY는 서버의 IP주소를 자동으로 찾아서 대입해주는 함수로서 서버의 주소를 소켓의 주소에 저장한다
	addr.sin_port = htons(PORT_NUM);//포트번호를 소켓의포트에 저장한다
	while(1) {
		printf("> ");
		fgets(msg, BUFSIZE-1, stdin);//입력을 받는다
		if(strncmp(msg, "quit\n",5) == 0) {
			break;
		}//quit입력시 종료
		sscanf(msg, "%d%[^0-9]%d", &left_num, op, &right_num);//숫자,숫자가 아닌문자,숫자 순으로 파싱한다
		memset((void *)&Inputdata, 0x00, sizeof(Inputdata));//Inputdata 메모리초기화
		Inputdata.left_num = htonl(left_num);
		Inputdata.right_num = htonl(right_num);//호스트바이트순서를 네트워크바이트순서로 변경
		Inputdata.op = op[0];
		addrlen = sizeof(addr);
		sendto(sockfd, (void *)&Inputdata, sizeof(Inputdata), 0,(struct sockaddr *)&addr, addrlen);
		//입력받은 값을 구조체에 저장시켜서 전송한다
		recvfrom(sockfd, (void *)&Inputdata, sizeof(Inputdata), 0,(struct sockaddr *)&recvaddr, &addrlen);
		//결과값을 담은 구조체를 읽는다
		printf("Result : %d %c %d = %d\n", ntohl(Inputdata.left_num), Inputdata.op,ntohl(Inputdata.right_num), ntohl(Inputdata.result));
		//결과를 프린트한다
	}
	close(sockfd);
}
