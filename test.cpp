#include <iostream>
#include "hikingSim.h"

using namespace std;


void t210(int num, int * b)//two to 10 //10진수 -> 2진수
{
	//int binary[16] = { 0, };

	int position = 0;
	while (1)
	{
		b[position] = num % 2;    // 2로 나누었을 때 나머지를 배열에 저장
		num = num / 2;             // 2로 나눈 몫을 저장

		position++;    // 자릿수 변경

		if (num == 0)    // 몫이 0이 되면 반복을 끝냄
			break;
	}

	// 배열의 요소를 역순으로 출력
	for (int i = 10 - 1; i >= 0; i--)
	{
		printf("%d", b[i]);
	}

	printf("\n");
}


int t22(int * b)//ten to 2 //2진수 -> 10진수
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
	//시프트 연산자로 정보부분 날려먹어서 파워만 남기기
	power = power << 6;
	power = power >> 6;

	if (effect & 1 << 10)//파워가 음수?
		power *= -1;
	if (effect & 1 << 11)//체력 회복?
	{
		printf("체력이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << 12)//기력 회복?
	{
		printf("기력이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << 13)//지능 증가?
	{
		printf("지능이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << 14)//민첩 증가?
	{
		printf("민첩성이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << 15)//렄휘이?
	{
		printf("운이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
}

void makeItem()
{
	int a;
	int binary[16] = { 0, };
	cout << "운빨 민첩 지능 기력 체력 음수" << endl;
	for (int i = 1; i <= 6; i++)
	{
		cin >> binary[16 - i];
	}
	for (int i = 0; i < 16; i++)
		cout << binary[i];
	cout << endl;
	cout << "파워";
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