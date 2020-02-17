#include "hikingSimUI.h"


void PrintData(char * fname)//ȭ�鿡 �ؽ�Ʈ ���� �Ѹ���
{
	ifstream fin;
	fin.open(fname);

	if (fin.good())
	{
		char line[100];

		while (!fin.eof())
		{
			fin.getline(line, 100);
			cout << line << endl;
		}

		fin.close();
	}
}

int MainMenu(char key, char cursor)//����ȭ��
{
	system("cls");//ȭ�� �����
	PrintData((char *)"Resources/main.txt");//�ΰ� ���

	//Ű
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 2 ? cursor++ : 0;
	else if (key == ENTER)
	{
		switch (cursor)
		{
		case 2://���� �Ѹ���
			system("cls");//ȭ�� �����
			PrintData((char *)"Resources/help.txt");//���� ���
			getche();//����ĥ������ ��ٸ�

		default://���� �̿ܿ��� Ŀ�� ����
			return cursor;
		}
		
	}

	printf(" %s ���ν���\n", cursor == 0 ? "��" : " ");
	printf(" %s �̾��ϱ�\n", cursor == 1 ? "��" : " ");
	printf(" %s ����\n", cursor == 2 ? "��" : " ");

	MainMenu(getch(), cursor);//������ ���� �ݺ�
}


int SubMainMenu(char * mt,char key, char cursor)//���� ����ȭ��
{
	system("cls");

	//Ű
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 3 ? cursor++ : 0;
	else if (key == LEFT)
		*mt > 0 ? *mt-=1 : 0;
	else if (key == RIGHT)
		*mt < MAX_MT_COUNT-1 ? *mt+=1 : 0;
	else if (key == ENTER)
		return cursor;//���� ������ Ŀ�� ����


	//�� �̹��� ���
	char path[20];
	sprintf(path,"Resources/m%d.txt", (*mt)+1);//���ڿ� ����� �Լ�
	PrintData(path);

	printf("\n\n�� �� ����Ű�� �� ����\n\n");
	printf(" %s ��Ÿ��\n", cursor == 0 ? "��" : " ");
	printf(" %s ���ǵ�\n", cursor == 1 ? "��" : " ");
	printf(" %s ����\n", cursor == 2 ? "��" : " ");
	printf("\n %s �����ϰ� ���θ޴���\n", cursor == 3 ? "��" : " ");

	SubMainMenu(mt,getch(), cursor);//������ ���� �ݺ�
}

int InGameMenu(char mapcode,char key, char cursor)//�ΰ��� ����ȭ��
{
	system("cls");
	//Ű
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 3 ? cursor++ : 0;
	else if (key == ENTER)
		return cursor;//���� ������ Ŀ�� ����

	//�� �̹��� ���
	char path[20];
	sprintf(path,"Resources/m%d.txt", mapcode+1);//���ڿ� ����� �Լ�
	PrintData(path);

	printf("\n\n %s ��� �����ϱ�\n", cursor == 0 ? "��" : " ");
	printf(" %s ���º���\n", cursor == 1 ? "��" : " ");
	printf(" %s ���ǵ�\n", cursor == 2 ? "��" : " ");

	InGameMenu(mapcode,getch(), cursor);//������ ���� �ݺ�
}


void PrintInven(Item * itemInfos,char * codes,char length,bool shopmode)//�κ��丮 �������� �̸��� ���� ���
{
	for (char i = 0; i < length; i++)
	{
		if (codes[i] == -1)return;
		printf("  |%10s |", codes[i] != -1 ? itemInfos[codes[i]].name : " ");//�̸�
		if (shopmode)//������忡���� ���ݵ� ������
			printf("%6d |", codes[i] != -1 ? itemInfos[codes[i]].cost : 0);
		printf("%70s |\n", codes[i] != -1 ? itemInfos[codes[i]].info : " ");//����
	}
}

char SelectAblePrintInven(Item * itemInfos, const char* name1, char * codes, char length, const char* name2, char * codes2, char length2,bool isSubUp, bool shopmode, char key, char cursor)//���ð����� �������
{
	system("cls");//Ŭ��~
	if (codes2 != nullptr && isSubUp)//���ʿ� 2��° �κ� ���
	{
		cout << name2 << "\n";
		PrintInven(itemInfos, codes2, length2);
		printf("\n  __________________________________________________________________________________________\n");
	}

	//Ű
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < length-1 ? cursor++ : 0;
	else if (key == ENTER)
		return cursor;//���� ������ �ε���(Ŀ��) ����
	else if (key == 'x' || key == 'X')//x�� ��ҳ� ��ü ��� ��¶�� �׷� �뵵
		return -1;
	else if (key == ESC)//Ż.��
		return -2;

	cout << name1 << "\n";
	for (char i = 0; i < length; i++)
	{
		//if (codes[i] == -1)continue;
		printf("%s ", i == cursor ? "��" : " ");//Ŀ��
		printf("|%10s |", codes[i]!=-1 ?itemInfos[codes[i]].name : " ");//�̸�
		if (shopmode)//������忡���� ���ݵ� ������
			printf("%6d |", codes[i] != -1 ? itemInfos[codes[i]].cost : 0);
		printf("%70s |\n", codes[i] != -1 ? itemInfos[codes[i]].info : " ");//����
	}

	if (codes2 != nullptr && !isSubUp)//�Ʒ��ʿ� 2��° �κ� ���
	{
		printf("\n  __________________________________________________________________________________________\n");
		cout << name2 << "\n";
		PrintInven(itemInfos, codes2, length2);
	}

	SelectAblePrintInven(itemInfos, name1, codes, length, name2,codes2, length2, isSubUp, shopmode, getch(), cursor);//���
}