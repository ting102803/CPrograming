
#include <stdio.h>
int Samechar(char A[],char B[]);

int main(void) {
	char first[10];
	char second[10];//ù��°�� �ι�° ���ڸ� ������ �迭����
	scanf("%s",first);
	scanf("%s",second);//�ΰ��� ���ڸ� �Է¹޴´�

	printf("�Է°� : %s %s\n",first,second);
	printf("����� : %d",Samechar(first,second));//�Լ��� ����

	return 0;
}

int Samechar(char A[],char B[]){//��ȯ�Ǵ� ���� int������ int�� �Լ� ����
int i;
int C=0;//��ȯ�Ǵ� �� ��Ÿ����
for(i=0;i<10;i++){
	if (A[i]==B[i])
		C++;//���� ù��° ���ڿ� �ι�° ������ ���ڿ��� ���ٸ� C�� 1�ø��� ���� ���ڿ��� �˻��Ѵ�.
	else break;}//���� �ٸ��ٸ� for���� ���� ���̻� �˻縦 ���� �ʵ����Ѵ�
return C;//C�� ����
}
