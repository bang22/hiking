 /*UI����*/
#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>

using namespace std;

//�� �� ����
#define MAX_MT_COUNT 6
//�κ��丮 �� �뷮
#define INVEN_MAX 20

enum Key//�Է¹޴� Ű
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,

	ESC = 27,
	ENTER = 13
};

/*
ȿ��

0~512���� �Ŀ�����(1<<9) , �� ���� ��Ʈ�� �÷��׷� ���
���� : 000010 0000110010  //ü�� 50ȸ��
*/

enum Effect//������&��Ȳ ȿ��
{
	M = 10,			//����?
	HEALTH,			//ü��ȸ��
	ENERGY,			//���ȸ��
	INTELLIGENCE,	//��������
	QUICKNESS,		//��ø�� ����
	LUCK				//�����
};

struct Item//������
{
	char code;							//������ �ڵ�
	char name[10];					//�̸�
	int cost;							//����

	char effectCount;				//ȿ������
	unsigned short  * effect;	//ȿ�� & �Ŀ�

	char info[100];					//����
};

extern void PrintData(char * fname);//������ ���

extern int MainMenu(char key=0, char cursor=0);//���θ޴�

//�ΰ���
extern int SubMainMenu(char * mt,char key=0, char cursor=0);//�ΰ��� �޴�
extern int InGameMenu(char mapcode,char key = 0, char cursor = 0);//�ΰ��� �޴�
extern void PrintInven(Item * itemInfos, char * codes, char length, bool shopmode = false);//�κ��丮 ���
extern char SelectAblePrintInven(Item * itemInfos,const char* name1 ,char * codes, char length, const char* name2= " ", char * codes2=nullptr,  char length2=0, bool isSubUp=false, bool shopmode = false, char key = 0, char cursor = 0);//���ð����� �κ����

