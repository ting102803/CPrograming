
#include <stdio.h>
char SmalltoCapital(char a);
char CappitaltoSmall(char a);

int main(void) {
	char Alpha[100];
	int i=0;
	scanf("%s",Alpha);
		for(i=0;Alpha[i]; i++){
			if((Alpha[i]>='a')&&(Alpha[i]<='z')){
				Alpha[i]=SmalltoCapital(Alpha[i]);//���� �ҹ��ڶ�� �빮�ڷ� �ٲ㼭 �ٽ� �����Ѵ�.
				}
			else{
				if((Alpha[i]>='A')&&(Alpha[i]<='Z')){
					Alpha[i]=CappitaltoSmall(Alpha[i]);//���� �빮�ڶ�� �ҹ��ڷ� �ٲ㼭 �ٽ� �����Ѵ�.
				}}}
		printf("%s\t",Alpha);//�����Ѱ��� ����Ѵ�

		return 0;
}

char SmalltoCapital(char a){
	return a = a - ' ';
};//�ҹ����� ���̴� �����ڵ�󿡼� 8������ 20��ŭ������ �� ������ �����̽� ��ŭ ���ذ� �����ϸ�ȴ�.
char CappitaltoSmall(char a){
	return a = a + ' ';
};//�빮���� ���̴� �����ڵ�󿡼� 8������ 20��ŭ������ �� ������ �����̽� ��ŭ �����ذ� �����ϸ�ȴ�.
