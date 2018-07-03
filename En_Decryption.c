#include <stdio.h>
#include <time.h>
void main()
{
	char a[20];//문자열 저장할 배열
	char temp[20];
	int en_de=0;//암호화 복호화 선택 스위치문
	int select = 0;//스위치문 조작 상수
	printf("암호화는 1번 복호화는 2번을 입력해주세요 \n");
	scanf_s("%d", &en_de, 1);
	switch (en_de)
	{
	case 1:
	{
		printf("암호화 하고싶은 문자열을 입력해주세요 \n");
		scanf_s("%s", a, 20);
		
		for (int i = 0; i < 20; i++) {
			temp[i] = a[i];
		}//문자열 임시저장
		while (select != -1) {
			printf("암호화할 방법을 택하세요 )) 1,2,3 종료 -1\n");
			for (int i = 0; i < 20; i++) {
				a[i] = temp[i];
			}
			scanf_s("%d", &select, 1);//방법 선택
			switch (select) {
			case 1:
				for (int i = 0; i < 20; i++) {
					if (a[i] == NULL) break;
					if (a[i] == 'z' || a[i] == 'Z') a[i] = a[i] - 25;
					else a[i] = a[i] + 1;
				}//1번방법은 인덱스에 +1씩옮기면되는데 z,Z일경우에는 a나 A로 가면된다
				printf("암호화된 문자열 : %s\n", a);
				break;
			case 2:
				for (int i = 0; i < 20; i++) {
					int t = i % 26;//26이상이면 알파벳을 1바퀴 순회함으로 26으로 나눈 나머지를 저장
					if (a[i] == NULL) break;
					if (65 <= a[i] && a[i] <= 90) {//대문자일경우
						if (a[i] + t <= 90)//바뀐 문자열이 Z를 넘지않을경우 
							a[i] = a[i] + t; //바로 더하면됨
						else {
							a[i] = a[i] + t - 26;//넘을경우에는 -26(알파벳 개수) 후에 더하면된다
						}
					}
					if (97 <= a[i] && a[i] <= 122) {//소문자일경우
						if (a[i] + t <= 122)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}//소문자일경우에도 동일하게한다.
					}
				}
				printf("암호화된 문자열 : %s\n", a);
				break;

			case 3:
				for (int i = 0; i < 20; i++) {
					int t = (i*i) % 26;//3번째 방법은 인덱스를 제곱해야함으로 t의값은 i^2%26으로 저장하면된다.
					if (a[i] == NULL) break;
					if (65 <= a[i] && a[i] <= 90) {//대문자일경우
						if (a[i] + t <= 90)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}
					}
					if (97 <= a[i] && a[i] <= 122) {//소문자일경우
						if (a[i] + t <= 122)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}//Z,z를 넘는경우를 확인하는건 방법2와 같다.
					}
				}
				printf("암호화된 문자열 : %s\n", a);
				break;
			case -1:
				system("pause");
				return 0;
				break;
			default:
				printf("숫자를 다시입력하세요 )) 1,2,3\n");

			}
		}}	break;
	case 2:
	{	printf("복호화 하고싶은 문자열을 입력해주세요 \n");
		scanf_s("%s", a, 20);
		for (int i = 0; i < 20; i++) {
			temp[i] = a[i];
		}
		while (select != -1) {
		printf("복호화할 방법을 택하세요 )) 1,2,3 종료 -1\n");
		scanf_s("%d", &select, 1);
		for (int i = 0; i < 20; i++) {
			a[i] = temp[i];
		}
		switch (select) {//복호화는 암호화의 역순이다.
		case 1:
			for (int i = 0; i < 20; i++) {
				if (a[i] == NULL) break;
				if (a[i] == 'a' || a[i] == 'A') a[i] = a[i] + 25;//z,Z대신 a,A,-대신 +로 바꾸면된다.
				else a[i] = a[i] - 1;
			}
			printf("복호화된 문자열 : %s\n", a);
			break;
		case 2:
			for (int i = 0; i < 20; i++) {
				int t = i % 26;
				if (a[i] == NULL) break;
				if (65 <= a[i] && a[i] <= 90) {//대문자의경우
					if (65 <= a[i] - t)//A이하로 내려갈경우
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
				else if (97 <= a[i] && a[i] <= 122) {//소문자의경우
					if (97 <= a[i] - t)//a이하로 내려갈경우
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
			}
			printf("복호화된 문자열 : %s\n", a);
			break;

		case 3:
			for (int i = 0; i < 20; i++) {
				int t = (i*i) % 26;
				if (a[i] == NULL) break;
				if (65 <= a[i] && a[i] <= 90) {
					if (65 <= a[i] - t)
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
				else if (97 <= a[i] && a[i] <= 122) {
					if (97 <= a[i] - t)
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
			}
			printf("복호화된 문자열 : %s\n", a);
			break;
		case -1:
			system("pause");
			return 0;
			break;
		default:
			printf("숫자를 다시입력하세요 )) 1,2,3\n");

		}
		}}break;
	default:
		printf("재실행하여 숫자를 다시 입력해주세요. )) 1,2");
	}
	system("pause");
	return 0;
}
