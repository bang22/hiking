/*게임 시스템 관련*/
#pragma once
#include <random>

#define MAX_INTEL 90
#define MAX_QUICK 90
#define MAX_LUCK 90

#include "hikingSimUI.h"

using namespace std;

struct UserData//유저 데이터
{
	char name[20];				//이름

	int money;					//머니

	char inven[INVEN_MAX];	//인벤토리
	char pocketCount;			//주머니 크기
	char * pocket;				//주머니

	short strenght;				//힘		= 체력최대치
	short stamina;				//지구력	= 기력최대치
	short intelligence;			//지능	= 함정 파훼능력
	short quickness;			//민첩	= 함정 피하기 능력
	short luck;						//운		= 운수좋은날
};

struct PlayerState//등산중일때 플레이어 상태
{
	short pos;//현재 위치	

	short health;			//체력
	short stamina;		//기력
	char intelligence;	//지능 
	char quickness;		//빠름
	char luck;				//운
};

enum MtState//등산 환경
{
	STEEP,		//가파름
	RAIN,		//비
	SNOW,		//눈
	NIGHT		//밤?
};

enum MTEffect
{
	A = 10 //
};

struct Situation
{
	char dialog[100];//다이얼로그
	char effectCountORcode;				//효과개수OR아이템 코드
	unsigned short  * effect;	//효과 & 파워
};

struct MtData//산 데이터
{
	char name[20];			//이름

	short defaultState;	//기본 상태
	short height;				//높이
	short defficulty;		//난이도
};

extern bool GameInit();//게임 초기화
extern void MainSeq();//게임 시퀸스
extern void EndGame();//가망 없음
