/*���� �ý��� ����*/
#pragma once
#include <random>

#define MAX_INTEL 90
#define MAX_QUICK 90
#define MAX_LUCK 90

#include "hikingSimUI.h"

using namespace std;

struct UserData//���� ������
{
	char name[20];				//�̸�

	int money;					//�Ӵ�

	char inven[INVEN_MAX];	//�κ��丮
	char pocketCount;			//�ָӴ� ũ��
	char * pocket;				//�ָӴ�

	short strenght;				//��		= ü���ִ�ġ
	short stamina;				//������	= ����ִ�ġ
	short intelligence;			//����	= ���� ���Ѵɷ�
	short quickness;			//��ø	= ���� ���ϱ� �ɷ�
	short luck;						//��		= ���������
};

struct PlayerState//������϶� �÷��̾� ����
{
	short pos;//���� ��ġ	

	short health;			//ü��
	short stamina;		//���
	char intelligence;	//���� 
	char quickness;		//����
	char luck;				//��
};

enum MtState//��� ȯ��
{
	STEEP,		//���ĸ�
	RAIN,		//��
	SNOW,		//��
	NIGHT		//��?
};

enum MTEffect
{
	A = 10 //
};

struct Situation
{
	char dialog[100];//���̾�α�
	char effectCountORcode;				//ȿ������OR������ �ڵ�
	unsigned short  * effect;	//ȿ�� & �Ŀ�
};

struct MtData//�� ������
{
	char name[20];			//�̸�

	short defaultState;	//�⺻ ����
	short height;				//����
	short defficulty;		//���̵�
};

extern bool GameInit();//���� �ʱ�ȭ
extern void MainSeq();//���� ������
extern void EndGame();//���� ����
