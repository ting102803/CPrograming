#include <stdio.h>
#include <time.h>
void main()
{
	char a[20];//���ڿ� ������ �迭
	char temp[20];
	int en_de=0;//��ȣȭ ��ȣȭ ���� ����ġ��
	int select = 0;//����ġ�� ���� ���
	printf("��ȣȭ�� 1�� ��ȣȭ�� 2���� �Է����ּ��� \n");
	scanf_s("%d", &en_de, 1);
	switch (en_de)
	{
	case 1:
	{
		printf("��ȣȭ �ϰ���� ���ڿ��� �Է����ּ��� \n");
		scanf_s("%s", a, 20);
		
		for (int i = 0; i < 20; i++) {
			temp[i] = a[i];
		}//���ڿ� �ӽ�����
		while (select != -1) {
			printf("��ȣȭ�� ����� ���ϼ��� )) 1,2,3 ���� -1\n");
			for (int i = 0; i < 20; i++) {
				a[i] = temp[i];
			}
			scanf_s("%d", &select, 1);//��� ����
			switch (select) {
			case 1:
				for (int i = 0; i < 20; i++) {
					if (a[i] == NULL) break;
					if (a[i] == 'z' || a[i] == 'Z') a[i] = a[i] - 25;
					else a[i] = a[i] + 1;
				}//1������� �ε����� +1���ű��Ǵµ� z,Z�ϰ�쿡�� a�� A�� ����ȴ�
				printf("��ȣȭ�� ���ڿ� : %s\n", a);
				break;
			case 2:
				for (int i = 0; i < 20; i++) {
					int t = i % 26;//26�̻��̸� ���ĺ��� 1���� ��ȸ������ 26���� ���� �������� ����
					if (a[i] == NULL) break;
					if (65 <= a[i] && a[i] <= 90) {//�빮���ϰ��
						if (a[i] + t <= 90)//�ٲ� ���ڿ��� Z�� ����������� 
							a[i] = a[i] + t; //�ٷ� ���ϸ��
						else {
							a[i] = a[i] + t - 26;//������쿡�� -26(���ĺ� ����) �Ŀ� ���ϸ�ȴ�
						}
					}
					if (97 <= a[i] && a[i] <= 122) {//�ҹ����ϰ��
						if (a[i] + t <= 122)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}//�ҹ����ϰ�쿡�� �����ϰ��Ѵ�.
					}
				}
				printf("��ȣȭ�� ���ڿ� : %s\n", a);
				break;

			case 3:
				for (int i = 0; i < 20; i++) {
					int t = (i*i) % 26;//3��° ����� �ε����� �����ؾ������� t�ǰ��� i^2%26���� �����ϸ�ȴ�.
					if (a[i] == NULL) break;
					if (65 <= a[i] && a[i] <= 90) {//�빮���ϰ��
						if (a[i] + t <= 90)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}
					}
					if (97 <= a[i] && a[i] <= 122) {//�ҹ����ϰ��
						if (a[i] + t <= 122)
							a[i] = a[i] + t;
						else {
							a[i] = a[i] + t - 26;
						}//Z,z�� �Ѵ°�츦 Ȯ���ϴ°� ���2�� ����.
					}
				}
				printf("��ȣȭ�� ���ڿ� : %s\n", a);
				break;
			case -1:
				system("pause");
				return 0;
				break;
			default:
				printf("���ڸ� �ٽ��Է��ϼ��� )) 1,2,3\n");

			}
		}}	break;
	case 2:
	{	printf("��ȣȭ �ϰ���� ���ڿ��� �Է����ּ��� \n");
		scanf_s("%s", a, 20);
		for (int i = 0; i < 20; i++) {
			temp[i] = a[i];
		}
		while (select != -1) {
		printf("��ȣȭ�� ����� ���ϼ��� )) 1,2,3 ���� -1\n");
		scanf_s("%d", &select, 1);
		for (int i = 0; i < 20; i++) {
			a[i] = temp[i];
		}
		switch (select) {//��ȣȭ�� ��ȣȭ�� �����̴�.
		case 1:
			for (int i = 0; i < 20; i++) {
				if (a[i] == NULL) break;
				if (a[i] == 'a' || a[i] == 'A') a[i] = a[i] + 25;//z,Z��� a,A,-��� +�� �ٲٸ�ȴ�.
				else a[i] = a[i] - 1;
			}
			printf("��ȣȭ�� ���ڿ� : %s\n", a);
			break;
		case 2:
			for (int i = 0; i < 20; i++) {
				int t = i % 26;
				if (a[i] == NULL) break;
				if (65 <= a[i] && a[i] <= 90) {//�빮���ǰ��
					if (65 <= a[i] - t)//A���Ϸ� ���������
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
				else if (97 <= a[i] && a[i] <= 122) {//�ҹ����ǰ��
					if (97 <= a[i] - t)//a���Ϸ� ���������
						a[i] = a[i] - t;
					else {
						a[i] = a[i] - t + 26;
					}
				}
			}
			printf("��ȣȭ�� ���ڿ� : %s\n", a);
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
			printf("��ȣȭ�� ���ڿ� : %s\n", a);
			break;
		case -1:
			system("pause");
			return 0;
			break;
		default:
			printf("���ڸ� �ٽ��Է��ϼ��� )) 1,2,3\n");

		}
		}}break;
	default:
		printf("������Ͽ� ���ڸ� �ٽ� �Է����ּ���. )) 1,2");
	}
	system("pause");
	return 0;
}
