#include <iostream>
#include "hikingSim.h"

using namespace std;


void t210(int num, int * b)//two to 10 //10���� -> 2����
{
	//int binary[16] = { 0, };

	int position = 0;
	while (1)
	{
		b[position] = num % 2;    // 2�� �������� �� �������� �迭�� ����
		num = num / 2;             // 2�� ���� ���� ����

		position++;    // �ڸ��� ����

		if (num == 0)    // ���� 0�� �Ǹ� �ݺ��� ����
			break;
	}

	// �迭�� ��Ҹ� �������� ���
	for (int i = 10 - 1; i >= 0; i--)
	{
		printf("%d", b[i]);
	}

	printf("\n");
}


int t22(int * b)//ten to 2 //2���� -> 10����
{
	int result = 0;
	for (int i = 15; i >= 0; i--)
	{
		cout << b[i];
		result *= 2;
		result += b[i];
	}
	return result;
}

void UseItem(short effect)
{
	short power = effect;
	//����Ʈ �����ڷ� �����κ� �����Ծ �Ŀ��� �����
	power = power << 6;
	power = power >> 6;

	if (effect & 1 << 10)//�Ŀ��� ����?
		power *= -1;
	if (effect & 1 << 11)//ü�� ȸ��?
	{
		printf("ü���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << 12)//��� ȸ��?
	{
		printf("����� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << 13)//���� ����?
	{
		printf("������ %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << 14)//��ø ����?
	{
		printf("��ø���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << 15)//������?
	{
		printf("���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
}

void makeItem()
{
	int a;
	int binary[16] = { 0, };
	cout << "� ��ø ���� ��� ü�� ����" << endl;
	for (int i = 1; i <= 6; i++)
	{
		cin >> binary[16 - i];
	}
	for (int i = 0; i < 16; i++)
		cout << binary[i];
	cout << endl;
	cout << "�Ŀ�";
	cin >> a;
	t210(a, binary);

	unsigned short effect = t22(binary);
	cout << endl << endl << t22(binary) << endl << endl;
	UseItem(t22(binary));
}
//
//void main()
//{
//	while (true)
//		makeItem();
//	
//}