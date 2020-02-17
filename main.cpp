#include "hikingSim.h"



int main()
{
	if (!GameInit())//초기화
	{
		printf("오락프로그람의 자료에 문제가 생겼읍니다.\n\n");
		getche();
	}
	else
		MainSeq();//게임 꼬

	EndGame();//동적할당 해제
	return 0;
}

