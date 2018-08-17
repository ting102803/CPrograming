#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int myaddress[4] = { 192, 168, 0, 1 };
int myport = 5650;//�ڽ��� IP�ּҿ� ��Ʈ �ּ� ����
int routeraddress[4] = { 168,188,123,60 };//������� IP�ּ� ����
struct packet {
	int sip[4];
	int dip[4];
	int sport;
	int dport;
};//packet ����ü ����
struct NAT {
	int priip[4];
	int priport;
	int extip[4];
	int extport;
};//Nat table�� �����ϱ����� ����
int print_packet(struct packet *Pack) {
	printf("====Packet information====\n");
	printf("Source IP : %d. %d. %d. %d\n", Pack->sip[0], Pack->sip[1], Pack->sip[2], Pack->sip[3]);
	printf("Dest   IP : %d. %d. %d. %d\n", Pack->dip[0], Pack->dip[1], Pack->dip[2], Pack->dip[3]);
	printf("Source PORT : %d\n", Pack->sport);
	printf("Dest   PORT : %d\n", Pack->dport);
	printf("==========================\n\n");
	return 1;
}//���� ��Ŷ�� ����ϱ����� �Լ� ����

int print_table(int *j,struct NAT *t) {
	int i;
		printf("===Current NAt Table Entry===\n");
		printf(" Private IP,Port             Public IP,Port\n");
	for (i = 0; i < *j; i++) {
		printf("%d. %d. %d. %d  %d       %d. %d. %d. %d %d\n", t[i].priip[0], t[i].priip[1], t[i].priip[2], t[i].priip[3],t[i].priport,t[i].extip[0], t[i].extip[1] , t[i].extip[2] , t[i].extip[3] , t[i].extport);
	};
	return 1;
}//Entry�� �߰��ɶ����� �߰��� ��Ʈ���� �����ִ� ���̺��� ����Ѵ�.
int sender_packet(struct packet  *Pack) {
	Pack->sip[0] = myaddress[0];
	Pack->sip[1] = myaddress[1];
	Pack->sip[2] = myaddress[2];
	Pack->sip[3] = myaddress[3];
	Pack->sport = myport;

	printf("Input destination IP address  ");
	fflush(stdin);
	scanf_s(" %d  %d  %d  %d", &Pack->dip[0], &Pack->dip[1], &Pack->dip[2], &Pack->dip[3]);
	printf("Input destination Port address  ");
	fflush(stdin);
	scanf_s(" %d", &Pack->dport);
	printf("Sender sends\n");
	fflush(stdin);
	print_packet(Pack);
	return 1;
};//��Ŷ�� ������ �����ּҿ� ��Ʈ��ȣ�� �Է� �ް� ���� ��Ŷ�� ���¸� ���
int router_private(int *j, struct NAT *t, struct packet *Pack) {
	int i;
	printf("NAT Receives Packet from Private Networks\n\n");
	print_packet(Pack);
	for (i = 0; i < *j; i++) {//Nat ���̺��� IP�ּҿ� ��Ʈ�� ��ġ�ϴ°� �ִ��� Ȯ���ϴ� for���̴�.
		if (Pack->dip[0] == t[i].extip[0] && Pack->dip[1] == t[i].extip[1] && Pack->dip[2] == t[i].extip[2] && Pack->dip[3] == t[i].extip[3] && Pack->dport == t[i].extport) {
			if (Pack->sip[0] == t[i].priip[0] && Pack->sip[1] == t[i].priip[1] && Pack->sip[2] == t[i].priip[2] && Pack->sip[3] == t[i].priip[3] && Pack->sport == t[i].priport){
				printf("Inputing IP&Port already exists in Nat table.\n\n");
				print_table(j, t);//����� ���̺��� ����Ѵ�.
				Pack->sip[0] = routeraddress[0];
				Pack->sip[1] = routeraddress[1];
				Pack->sip[2] = routeraddress[2];
				Pack->sip[3] = routeraddress[3];//��Ŷ�� �ҽ� �ּҸ� ����� �ּҷ� �ٲ۴�
				printf("NAT Sends Packet to External Network\n\n");
				print_packet(Pack);

				printf("Receiver Recevies Packet \n\n");
				print_packet(Pack);//�޴»�����忡�� ���� ��Ŷ�� ����Ѵ�.
			
				return 1;
			
			}
		}
	}
	printf("New Entry!!\n\n");
	t[*j].extip[0] = Pack->dip[0];
	t[*j].extip[1] = Pack->dip[1];
	t[*j].extip[2] = Pack->dip[2];
	t[*j].extip[3] = Pack->dip[3];
	t[*j].extport = Pack->dport;

	t[*j].priip[0] = Pack->sip[0];
	t[*j].priip[1] = Pack->sip[1];
	t[*j].priip[2] = Pack->sip[2];
	t[*j].priip[3] = Pack->sip[3];
	t[*j].priport = Pack->sport;
	*j=*j+1;//���� ��Ŷ�� ���۵���,IP�� Port�� Nat���̺� �����Ѵ�.
	//�׸��� entry�� ������ ���������ش�

	print_table(j,t);//����� ���̺��� ����Ѵ�.
	Pack->sip[0] = routeraddress[0];
	Pack->sip[1] = routeraddress[1];
	Pack->sip[2] = routeraddress[2];
	Pack->sip[3] = routeraddress[3];//��Ŷ�� �ҽ� �ּҸ� ����� �ּҷ� �ٲ۴�
	printf("NAT Sends Packet to External Network\n\n");
	print_packet(Pack);

	printf("Receiver Recevies Packet \n\n");
	print_packet(Pack);//�޴»�����忡�� ���� ��Ŷ�� ����Ѵ�.
	return 1;
};
int mypacket_to_recevieoacket(struct packet *Pack) {
	int temp[4];
	int k=0;
	for(k=0;k<4;k++){
	temp[k] = Pack->dip[k];
	Pack->dip[k] = Pack->sip[k];
	Pack->sip[k] = temp[k];
	}
	k = Pack->dport;
	Pack->dport = Pack->sport;
	Pack->sport = k;
	return 1;
};//�޴»���� ��Ŷ�� �ް� reply�Ҷ� �ۼ��� ��Ŷ������ �ٲٴ� �Լ��̴�.
int receiver_packet(struct packet *Pack) {
	
	printf("Receiver Sends Packet\n\n");
	print_packet(Pack);

	return 1;
};//��������� ��Ŷ �������Ҷ� ��Ŷ�� �����̴�.
int router_public(int *j, struct NAT *t, struct packet *Pack) {
	printf("NAT Receives Packet from External Network\n\n");
	print_packet(Pack);
	printf("NAT Sends Packet to Private Network\n");
	int i = 0;
	for (i = 0; i < *j; i++) {//Nat ���̺��� IP�ּҿ� ��Ʈ�� ��ġ�ϴ°� �ִ��� Ȯ���ϴ� for���̴�.
		if (Pack->sip[0] == t[i].extip[0] && Pack->sip[1] == t[i].extip[1] && Pack->sip[2] == t[i].extip[2] && Pack->sip[3] == t[i].extip[3] && Pack->sport == t[i].extport) {
			Pack->dip[0] = t[i].priip[0];//�ܺο��� ���� ��Ŷ�� �ҽ�IP,��Ʈ�� ���̺� �ִ� �ܺ�IP,��Ʈ�� ��ġ�ϴ°� �ִٸ� �������� ���� IP�� ��Ʈ�� �������ش�. 
			Pack->dip[1] = t[i].priip[1];
			Pack->dip[2] = t[i].priip[2];
			Pack->dip[3] = t[i].priip[3];
			Pack->dport = t[i].priport;
			printf("Find Corrected Nat Table!\n");
			print_packet(Pack);
			return 1;
		}
		else if (i == *j-1) {
			printf("No corret Nat Table\n");
			return 0;
		}
	}
	return 0;
};

int main()
{
	struct packet *mypacket = (struct packet *)malloc(sizeof(struct packet));
	struct NAT table[10];
	int entry = 0;
	char ch = 'Y';
	while (1) {
		sender_packet(mypacket);
		router_private(&entry, table, mypacket);
		mypacket_to_recevieoacket(mypacket);
		receiver_packet(mypacket);
		router_public(&entry, table, mypacket);
		printf("Continue (Y/N)\n");
		fflush(stdin);
		scanf_s(" %c", &ch);
		if (ch == 'N' || ch == 'n')
			return 1;
	}//��� �����ҷ��� y�Է�,n�� �Է��ϸ� ���α׷��� ����ȴ�.
}