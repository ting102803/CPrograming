
#include <stdio.h>
char SmalltoCapital(char a);
char CappitaltoSmall(char a);

int main(void) {
	char Alpha[100];
	int i=0;
	scanf("%s",Alpha);
		for(i=0;Alpha[i]; i++){
			if((Alpha[i]>='a')&&(Alpha[i]<='z')){
				Alpha[i]=SmalltoCapital(Alpha[i]);//만약 소문자라면 대문자로 바꿔서 다시 저장한다.
				}
			else{
				if((Alpha[i]>='A')&&(Alpha[i]<='Z')){
					Alpha[i]=CappitaltoSmall(Alpha[i]);//만약 대문자라면 소문자로 바꿔서 다시 저장한다.
				}}}
		printf("%s\t",Alpha);//저장한값을 출력한다

		return 0;
}

char SmalltoCapital(char a){
	return a = a - ' ';
};//소문자의 차이는 유니코드상에서 8진수로 20만큼임으로 그 차이인 스페이스 만큼 빼준걸 리턴하면된다.
char CappitaltoSmall(char a){
	return a = a + ' ';
};//대문자의 차이는 유니코드상에서 8진수로 20만큼임으로 그 차이인 스페이스 만큼 더해준걸 리턴하면된다.
