#include "hikingSim.h"

//아이템, 플레이어 관련 전역변수

static MtData mtdatas[MAX_MT_COUNT];

static char itemCount;//총 아이템 개수
static Item * itemInfos;//아이템 정보들

static char goodSituationCount;//좋은 상황 개수
static Situation *  goodSituations;//조흔 상황
static char badSituationCount;//나쁜 상황 개수
static Situation *  badSituations;//나픈 상황
static char getSituationCount;//얻는 상황 개수



static UserData player;//유저 데이터



//CHO기火 !

static bool LoadItems()//아이템 목록 초기화
{
	//아이템 정보 불러오기
	ifstream fin;
	fin.open("Resources/items.txt");

	if (!fin.good()) return false;//파일에 문제가 생기면 false


	int line, cost, effectCount;//파일의 줄 수와 각 아이템의 효과개수
	fin >> line;

	itemCount = line;
	itemInfos = new Item[line];//아이템 개수

	for (char i = 0; i < line; i++)
	{
		itemInfos[i].code = i;
		fin >> itemInfos[i].name >> cost >> effectCount;//이름,가격,이펙트 개수

		printf("1\n");

		itemInfos[i].cost = cost;
		itemInfos[i].effectCount = effectCount;
		itemInfos[i].effect = new unsigned short[effectCount];//이펙트 개수
		for (char j = 0; j < effectCount; j++)//이펙트들
		{
			fin >> itemInfos[i].effect[j];
		}
		fin.getline(itemInfos[i].info, 100);//정보
	}

	fin.close();

	return true;
}

static bool LoadMtData()//산 정보목록 초기화
{
	ifstream fin;
	fin.open("Resources/mountain.txt");
	if (!fin.good())//파일이 열리지 않으면 그만
	{
		return false;
	}

	for (char i = 0; i < MAX_MT_COUNT; i++)
	{
		//이름 , 산의 상태 , 높이 , 난이도
		fin >> mtdatas[i].name >> mtdatas[i].defaultState >> mtdatas[i].height >> mtdatas[i].defficulty;
	}

	return true;
}

static bool LoadSituationData()//시츄에이션들 불러오기
{
	//아이템 정보 불러오기
	ifstream fin;
	fin.open("Resources/situation.txt");

	if (!fin.good()) return false;//파일에 문제가 생기면 false


	int line, effectCount;//상황별 개수와 상황별 효과개수

	//좋은 상황
	fin >> line;
	goodSituationCount = line;
	goodSituations = new Situation[line];//아이템 개수

	for (char i = 0; i < line; i++)
	{
		fin >> effectCount;//이펙트 개수=
		goodSituations[i].effectCountORcode = effectCount;
		goodSituations[i].effect = new unsigned short[effectCount];//이펙트 개수
		for (char j = 0; j < effectCount; j++)//이펙트들
		{
			fin >> goodSituations[i].effect[j];
		}
		fin.getline(goodSituations[i].dialog, 100);//상황설명
	}

	//나쁜 상황
	fin >> line;
	badSituationCount = line;
	badSituations = new Situation[line];//아이템 개수

	for (char i = 0; i < line; i++)
	{
		fin >> effectCount;//이펙트 개수
		badSituations[i].effectCountORcode = effectCount;
		badSituations[i].effect = new unsigned short[effectCount];//이펙트 개수
		for (char j = 0; j < effectCount; j++)//이펙트들
		{
			fin >> badSituations[i].effect[j];
		}
		fin.getline(badSituations[i].dialog, 100);//상황설명
	}

	fin.close();

	return true;
}

static void InitUserData()//저장된 유저데이터가 없을때 초기화
{
	printf("이름을 입력하세요. : ");
	cin >> player.name;//이름 받기

	for (char i = 0; i < INVEN_MAX; i++)//인벤 초기화
		player.inven[i] = -1;

	player.pocket = new char;
	player.pocketCount = 0;
	player.money = 10000;//만원!
	player.strenght = player.stamina = 100;
	player.intelligence = player.quickness = player.luck = 10;//능력치 초기화
}

static void LoadUserData()//유저데이터 불러오기
{
	ifstream fin;
	fin.open("save.txt");
	if (!fin.good())//파일이 열리지 않으면 새로 만듬
	{
		printf("세이브 파일에 문제가 생겼습니다... \n\n");
		InitUserData();
		return;
	}

	//인벤토리 불러오기
	int code;
	for (char i = 0; i < INVEN_MAX; i++)
	{
		fin >> code;
		player.inven[i] = code;
	}
	//능력치 불러오기
	fin >> player.strenght >> player.stamina >> player.intelligence >> player.quickness >> player.luck;
}

static bool SaveUserData()//데이터 세이브
{
	ofstream fout;
	fout.open("save.txt");
	if (fout.good() == false)return false;

	for (char i = 0; i < INVEN_MAX; i++)//아이템 저장하기
	{
		fout << (int)player.inven[i] << " ";
	}
	//능력치 저장하기
	fout << "\n" << player.strenght << player.stamina << player.intelligence << player.quickness << player.luck;

	fout.close();

	return true;
}

bool GameInit()
{
	if (!LoadItems())return false;
	if (!LoadMtData())return false;
	if (!LoadSituationData())return false;

	srand(time(NULL));
}

//인게임


static void UseEffect(PlayerState * data, short effect, float EffectScale = 1)//아이템 사용
{
	short power = effect;
	//시프트 연산자로 정보부분 날려먹어서 파워만 남기기
	power = power << 6;
	power = power >> 6;
	power *= EffectScale;//이펙트 스케일 곱하기

	if (effect & 1 << Effect::M)//파워가 음수?
		power = -power;
	if (effect & 1 << Effect::HEALTH)//체력 회복?
	{
		data->health += power;
		printf("체력이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << Effect::ENERGY)//기력 회복?
	{
		data->stamina += power;
		printf("기력이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << Effect::INTELLIGENCE)//지능 증가?
	{
		data->intelligence += power;
		printf("지능이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << Effect::QUICKNESS)//민첩 증가?
	{
		data->quickness += power;
		printf("민첩성이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
	if (effect & 1 << Effect::LUCK)//렄휘이?
	{
		data->luck += power;
		printf("운이 %d %s갔습니다.\n", power, power < 0 ? "내려" : "올라");
	}
}

static void UseItem(PlayerState * data, char index)
{
	for (char i = 0; i < itemInfos[player.pocket[index]].effectCount; i++)//아이템 효과들 적용
	{
		UseEffect(data, itemInfos[player.pocket[index]].effect[i]);
	}
}

//아이템//

static void InvenSort(char inven[], char left, char right)//퀵 소트 //정렬할 배열,구역의 왼쪽,구역의 오른쪽
{
	char i = left, j = right;//i=왼쪽 ,j=오른쪽
	char pivot = inven[(left + right) / 2];//기준점은 구역의 중간
	char temp;
	do
	{
		while (inven[i] < pivot)//왼쪽이 피벗보다 (어차피)작다면 스킵
			i++;
		while (inven[j] > pivot)//오른쪽이 피벗보다 (어차피)크다면 스킵
			j--;
		if (i <= j)//위에서 걸러지지 못한건 서로 교체
		{
			temp = inven[i];
			inven[i] = inven[j];
			inven[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);//왼쪽이랑 오른쪽이랑 만날때까지 반복

	//구역나눠 재귀
	if (left < j)//왼쪽 구역
		InvenSort(inven, left, j);

	if (i < right)//오른쪽 구역
		InvenSort(inven, i, right);
}

static void AddPocketItem(char code)//포켓아이템 추가하기
{
	//조금더 큰 주머니 만들기
	player.pocketCount++;
	char * newPocket;
	newPocket = new char[player.pocketCount];

	//조금더 큰 주머니에 기존 주머니 내용물 넣기
	for (char i = 0; i < player.pocketCount - 1; i++)
		newPocket[i] = player.pocket[i];
	newPocket[player.pocketCount - 1] = code;//새 주머니의 끝에 새 아이템 추가

	InvenSort(newPocket, 0, player.pocketCount - 1);//정렬

	//기존 주머니 지우고 새 주머니로 바꿔치기
	delete player.pocket;
	player.pocket = newPocket;
	printf("\n\n\n%d\n\n\n", player.pocket[0]);

}

static void DeletePocketItem(char index)//포켓아이템 지우기
{
	//조금 더 작은 주머니 만들기
	player.pocketCount--;
	char * newPocket;
	newPocket = new char[player.pocketCount];

	char j = 0;
	for (char i = 0; i < player.pocketCount; i++)//새주머니에 기존주머니 내용물 넣기
	{
		if (index == j)//인덱스는 스킵
			j++;

		newPocket[i] = player.pocket[j];
		j++;
	}
	//기존 주머니 지우고 새주머니로 바꿔치기
	delete player.pocket;
	player.pocket = newPocket;
}

static void Packing()//짐싸기
{
	bool x = true;
	char r;
	while (true)
	{
		while (true)
		{
			if (x)//인벤선택
				r = (SelectAblePrintInven(itemInfos, "인벤토리", player.inven, INVEN_MAX, "방향키와 엔터로 선택, ESC로 나가기\n\n가방", player.pocket, player.pocketCount, true));
			else//주머니 선택
				r = (SelectAblePrintInven(itemInfos, "방향키와 엔터로 선택, ESC로 나가기\n\n가방", player.pocket, player.pocketCount, "인벤토리", player.inven, INVEN_MAX, false));
			if (r == -1)//x (교체)
				x = !x;
			else if (r == -2)//이스케이프
				return;
			else
				break;//선택완료
		}

		if (x)//인벤에서 포켓으로 옮기기
		{
			//포켓에 추가
			AddPocketItem(player.inven[r]);


			//인벤에서 제거
			player.inven[r] = -1;
			//-1(비어있는거) 맨 뒤로 보내기
			char last = INVEN_MAX - 1;
			char temp;
			for (char x = 0; x < last; x++)
				if (player.inven[x] == -1)
				{
					//교환
					temp = player.inven[x];
					player.inven[x] = player.inven[last];
					player.inven[last] = temp;
					while (player.inven[last] == -1) last--;//쓸데없는 계산방지
					x--;//중복으로 인한 오류방지
				}
			InvenSort(player.inven, 0, last);//나머지 부분 소트

		}
		else//포켓에서 인벤으로 옮gigi
		{
			//인벤에 추가
			if (player.inven[INVEN_MAX - 1] == -1)
			{
				player.inven[INVEN_MAX - 1] = player.pocket[r];

				//-1(비어있는거) 맨 뒤로 보내기
				char last = INVEN_MAX - 1;
				char temp;
				for (char x = 0; x < last; x++)
					if (player.inven[x] == -1)
					{
						//교환
						temp = player.inven[x];
						player.inven[x] = player.inven[last];
						player.inven[last] = temp;
						while (player.inven[last] == -1) last--;//쓸데없는 계산방지
						x--;//중복으로 인한 오류방지
					}
				InvenSort(player.inven, 0, last);//나머지 부분 소트

				//포켓에 있던 아이템 삭제
				DeletePocketItem(r);
			}
		}
	}
}

void Shop()//상점
{
	char * a = new char[itemCount];
	for (int i = 0; i < itemCount; i++)
		a[i] = i;
	while (true)
	{
		char code = SelectAblePrintInven(itemInfos, "상점", a, itemCount, "방향키와 엔터로 선택, ESC로 나가기\n\n인벤토리", player.inven, 20, true, true);
		if (code == -1)//(원래는 교체)
			continue;
		else if (code == -2)//이스케이프
			break;

		if (player.money >= itemInfos[code].cost && player.inven[INVEN_MAX - 1] == -1)
		{
			player.money -= itemInfos[code].cost;//돈 내기
			player.inven[INVEN_MAX - 1] = code;

			//-1(비어있는거) 맨 뒤로 보내기
			char last = INVEN_MAX - 1;
			char temp;
			for (char x = 0; x < last; x++)
				if (player.inven[x] == -1)
				{
					//교환
					temp = player.inven[x];
					player.inven[x] = player.inven[last];
					player.inven[last] = temp;
					while (player.inven[last] == -1) last--;//쓸데없는 계산방지
					x--;//중복으로 인한 오류방지
				}
			InvenSort(player.inven, 0, last);//나머지 부분 소트
		}

	}
	delete[] a;
}

//상황들

static void PlaySituation(PlayerState * state)//시추에이션 실행
{
	Situation sit;
	float scale = 1;
	if (state->luck > rand() % 100)//운이 좋으면
	{
		sit = goodSituations[rand() % goodSituationCount];
		printf("%s\n", sit.dialog);
		if (state->intelligence > rand() % 100)
		{
			printf("\n그리고 당신은 머리를 써서 좋은 상황을 더 좋게 만들었다.\n\n");
			scale = 2;
		}
	}
	else//나쁘면
	{
		sit = badSituations[rand() % badSituationCount];
		printf("%s\n", sit.dialog);
		if (state->quickness > rand() % 100)//그나마 운이 좋으면
		{
			printf("\n하지만 적당히 피해서 절반의 피해만을 입었다. \n\n");
			scale = 0.5;
		}
	}

	for (int i = 0; i < sit.effectCountORcode; i++)
		UseEffect(state, sit.effect[i], scale);
}

//천방지축어리둥절빙글빙글돌아가는게임의사이클S~♬

void InGameSeq(char map)//인게임 시퀸스 (그냥 게임 돌아가는거)
{
	//능력치 초기화
	PlayerState * state = new PlayerState();
	state->pos = 0;
	state->health = player.strenght;
	state->stamina = player.stamina;
	state->intelligence = player.intelligence;
	state->quickness = player.quickness;
	state->luck = player.luck;

	short mtState;//등산 환경
	mtState = mtdatas[map].defaultState;

	while (true)
	{
		switch (InGameMenu(map))
		{
			system("cls");
		case 1://상태 보기

			printf("현재 상태\n\n");
			printf("\t체력 : %d\n", state->health);
			printf("\t기력 : %d\n", state->stamina);
			printf("\t지능 : %d\n", state->intelligence);
			printf("\t민첩 : %d\n", state->quickness);
			printf("\t운빨 : %d\n", state->luck);
			getche();

			continue;

		case 2://아이템 사용
			char a = SelectAblePrintInven(itemInfos, "방향키와 엔터로 사용할 아이템을 선택하세요. \n나가려면 ESC키를 눌러주세요", player.pocket, player.pocketCount);
			if (a == -2) continue;
			printf("%d", a);
			UseItem(state, a);//아이템 이펙트 사용
			DeletePocketItem(a);//아이템 목록에서 지우기
			break;
		}

		//가자

		//난이도에 따라 무슨 일이 일어남
		if (mtdatas[map].defficulty < rand() % 100)
			PlaySituation(state);
		else
			printf("아무일도 없이 진행하였다.\n");

		state->pos++;
		if (state->pos == mtdatas[map].height)//정상
		{
			printf("\n\n정상에 도착했다.. 기력이 쏫아오른다!\n");
			UseEffect(state, 4126);//기력 30회복
		}
		else if (state->pos == mtdatas[map].height * 2)//등산 끝
		{
			break;
		}
		else
		{
			printf("\n\n당신은 힘들어도 %s 향해 발걸음을 이어나갔다.\n", state->pos < mtdatas[map].height ? "정상을" : "입구를");
			UseEffect(state, 5130);//기력 10하락
			printf("\n%s까지 %d남음\n", state->pos < mtdatas[map].height ? "정상" : "입구",
				state->pos < mtdatas[map].height ? mtdatas[map].height - state->pos : mtdatas[map].height * 2 - state->pos);
		}

		getche();
	}

	delete state;
	delete player.pocket;//주머니 털기
	player.pocket = new char[0];

	printf("\n\n등산이 드디어 ! 끝났다 !\n\n");
	printf("<세계비밀등산가및등산로개척가지원협회아시아지부> 에서 %d원 만큼 지원금이 나왔다.", mtdatas[map].height * mtdatas[map].defficulty * 10);
	getche();
}

void MainSeq()//메인 시퀸스
{
	while (true)
	{
		switch (MainMenu())
		{
		case 0://새로시작
			InitUserData();
			break;
		case 1://이어하기
			LoadUserData();
			break;
		}
		while (true)
		{
			char mt = 0;
			switch (SubMainMenu(&mt))
			{
			case 0://게임 시작
				InGameSeq(mt);

				break;

			case 1://인벤토리 보기
				Packing();
				break;

			case 2://상점
				Shop();
				break;

			case 3://저장하고 메인메뉴로 돌아가기
				if (!SaveUserData())
				{
					printf("저장에 실패하였습니다.");
					return;
				}
				goto ESCAPE;//goto는 쓰라고 있는 것!
			}
		}

	ESCAPE:;//스위치문탈출
	}
}

void EndGame()//가망 없음
{
	delete[] player.pocket;
	for (char a = 0; a < itemCount; a++)//아템들
	{
		delete[] itemInfos[a].effect;
	}
	delete[] itemInfos;

	for (char a = 0; a < goodSituationCount; a++)//조은 상황
	{
		delete[] goodSituations[a].effect;
	}
	delete[] goodSituations;

	for (char a = 0; a < badSituationCount; a++)//나픈 상황
	{
		delete[] badSituations[a].effect;
	}
	delete[] badSituations;
}