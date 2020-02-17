#include "hikingSimUI.h"


void PrintData(char * fname)//화면에 텍스트 파일 뿌리기
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

int MainMenu(char key, char cursor)//메인화면
{
	system("cls");//화면 지우기
	PrintData((char *)"Resources/main.txt");//로고 출력

	//키
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 2 ? cursor++ : 0;
	else if (key == ENTER)
	{
		switch (cursor)
		{
		case 2://도움말 뿌리기
			system("cls");//화면 지우기
			PrintData((char *)"Resources/help.txt");//도움말 출력
			getche();//엔터칠때까지 기다림

		default://도움말 이외에는 커서 리턴
			return cursor;
		}
		
	}

	printf(" %s 새로시작\n", cursor == 0 ? "▷" : " ");
	printf(" %s 이어하기\n", cursor == 1 ? "▷" : " ");
	printf(" %s 도움말\n", cursor == 2 ? "▷" : " ");

	MainMenu(getch(), cursor);//끝날때 까지 반복
}


int SubMainMenu(char * mt,char key, char cursor)//서브 메인화면
{
	system("cls");

	//키
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 3 ? cursor++ : 0;
	else if (key == LEFT)
		*mt > 0 ? *mt-=1 : 0;
	else if (key == RIGHT)
		*mt < MAX_MT_COUNT-1 ? *mt+=1 : 0;
	else if (key == ENTER)
		return cursor;//엔터 누르면 커서 리턴


	//산 이미지 출력
	char path[20];
	sprintf(path,"Resources/m%d.txt", (*mt)+1);//문자열 만드는 함수
	PrintData(path);

	printf("\n\n좌 우 방향키로 산 고르기\n\n");
	printf(" %s 산타기\n", cursor == 0 ? "▷" : " ");
	printf(" %s 물건들\n", cursor == 1 ? "▷" : " ");
	printf(" %s 상점\n", cursor == 2 ? "▷" : " ");
	printf("\n %s 저장하고 메인메뉴로\n", cursor == 3 ? "▷" : " ");

	SubMainMenu(mt,getch(), cursor);//끝날때 까지 반복
}

int InGameMenu(char mapcode,char key, char cursor)//인게임 메인화면
{
	system("cls");
	//키
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < 3 ? cursor++ : 0;
	else if (key == ENTER)
		return cursor;//엔터 누르면 커서 리턴

	//산 이미지 출력
	char path[20];
	sprintf(path,"Resources/m%d.txt", mapcode+1);//문자열 만드는 함수
	PrintData(path);

	printf("\n\n %s 계속 진행하기\n", cursor == 0 ? "▷" : " ");
	printf(" %s 상태보기\n", cursor == 1 ? "▷" : " ");
	printf(" %s 물건들\n", cursor == 2 ? "▷" : " ");

	InGameMenu(mapcode,getch(), cursor);//끝날때 까지 반복
}


void PrintInven(Item * itemInfos,char * codes,char length,bool shopmode)//인벤토리 아이템의 이름과 정보 출력
{
	for (char i = 0; i < length; i++)
	{
		if (codes[i] == -1)return;
		printf("  |%10s |", codes[i] != -1 ? itemInfos[codes[i]].name : " ");//이름
		if (shopmode)//상점모드에서는 가격도 보여줌
			printf("%6d |", codes[i] != -1 ? itemInfos[codes[i]].cost : 0);
		printf("%70s |\n", codes[i] != -1 ? itemInfos[codes[i]].info : " ");//정보
	}
}

char SelectAblePrintInven(Item * itemInfos, const char* name1, char * codes, char length, const char* name2, char * codes2, char length2,bool isSubUp, bool shopmode, char key, char cursor)//선택가능한 정보출력
{
	system("cls");//클린~
	if (codes2 != nullptr && isSubUp)//위쪽에 2번째 인벤 출력
	{
		cout << name2 << "\n";
		PrintInven(itemInfos, codes2, length2);
		printf("\n  __________________________________________________________________________________________\n");
	}

	//키
	if (key == UP)
		cursor > 0 ? cursor-- : 0;
	else if (key == DOWN)
		cursor < length-1 ? cursor++ : 0;
	else if (key == ENTER)
		return cursor;//엔터 누르면 인덱스(커서) 리턴
	else if (key == 'x' || key == 'X')//x는 취소나 교체 등등 어쨋든 그런 용도
		return -1;
	else if (key == ESC)//탈.출
		return -2;

	cout << name1 << "\n";
	for (char i = 0; i < length; i++)
	{
		//if (codes[i] == -1)continue;
		printf("%s ", i == cursor ? "▷" : " ");//커서
		printf("|%10s |", codes[i]!=-1 ?itemInfos[codes[i]].name : " ");//이름
		if (shopmode)//상점모드에서는 가격도 보여줌
			printf("%6d |", codes[i] != -1 ? itemInfos[codes[i]].cost : 0);
		printf("%70s |\n", codes[i] != -1 ? itemInfos[codes[i]].info : " ");//정보
	}

	if (codes2 != nullptr && !isSubUp)//아래쪽에 2번째 인벤 출력
	{
		printf("\n  __________________________________________________________________________________________\n");
		cout << name2 << "\n";
		PrintInven(itemInfos, codes2, length2);
	}

	SelectAblePrintInven(itemInfos, name1, codes, length, name2,codes2, length2, isSubUp, shopmode, getch(), cursor);//재귀
}