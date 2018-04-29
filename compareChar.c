
#include <stdio.h>
int Samechar(char A[],char B[]);

int main(void) {
	char first[10];
	char second[10];//첫번째와 두번째 문자를 저장할 배열선언
	scanf("%s",first);
	scanf("%s",second);//두가지 문자를 입력받는다

	printf("입력값 : %s %s\n",first,second);
	printf("결과값 : %d",Samechar(first,second));//함수를 실행

	return 0;
}

int Samechar(char A[],char B[]){//반환되는 값은 int임으로 int형 함수 선언
int i;
int C=0;//반환되는 값 나타낸다
for(i=0;i<10;i++){
	if (A[i]==B[i])
		C++;//만약 첫뻔째 문자와 두번째 문자의 문자열이 같다면 C를 1늘리고 다음 문자열도 검사한다.
	else break;}//만약 다르다면 for문을 나와 그이상 검사를 하지 않도록한다
return C;//C를 리턴
}
