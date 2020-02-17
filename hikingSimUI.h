 /*UI관련*/
#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include <windows.h>

using namespace std;

//총 산 개수
#define MAX_MT_COUNT 6
//인벤토리 총 용량
#define INVEN_MAX 20

enum Key//입력받는 키
{
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGHT = 77,

	ESC = 27,
	ENTER = 13
};

/*
효과

0~512까지 파워지정(1<<9) , 그 이후 비트를 플래그로 사용
예시 : 000010 0000110010  //체력 50회복
*/

enum Effect//아이템&상황 효과
{
	M = 10,			//음수?
	HEALTH,			//체력회복
	ENERGY,			//기력회복
	INTELLIGENCE,	//지능증가
	QUICKNESS,		//민첩성 증가
	LUCK				//운빨증가
};

struct Item//아이템
{
	char code;							//아이템 코드
	char name[10];					//이름
	int cost;							//가격

	char effectCount;				//효과개수
	unsigned short  * effect;	//효과 & 파워

	char info[100];					//설명
};

extern void PrintData(char * fname);//데이터 출력

extern int MainMenu(char key=0, char cursor=0);//메인메뉴

//인게임
extern int SubMainMenu(char * mt,char key=0, char cursor=0);//인게임 메뉴
extern int InGameMenu(char mapcode,char key = 0, char cursor = 0);//인게임 메뉴
extern void PrintInven(Item * itemInfos, char * codes, char length, bool shopmode = false);//인벤토리 출력
extern char SelectAblePrintInven(Item * itemInfos,const char* name1 ,char * codes, char length, const char* name2= " ", char * codes2=nullptr,  char length2=0, bool isSubUp=false, bool shopmode = false, char key = 0, char cursor = 0);//선택가능한 인벤출력

