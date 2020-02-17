#include "hikingSim.h"

//������, �÷��̾� ���� ��������

static MtData mtdatas[MAX_MT_COUNT];

static char itemCount;//�� ������ ����
static Item * itemInfos;//������ ������

static char goodSituationCount;//���� ��Ȳ ����
static Situation *  goodSituations;//���� ��Ȳ
static char badSituationCount;//���� ��Ȳ ����
static Situation *  badSituations;//���� ��Ȳ
static char getSituationCount;//��� ��Ȳ ����



static UserData player;//���� ������



//CHO���� !

static bool LoadItems()//������ ��� �ʱ�ȭ
{
	//������ ���� �ҷ�����
	ifstream fin;
	fin.open("Resources/items.txt");

	if (!fin.good()) return false;//���Ͽ� ������ ����� false


	int line, cost, effectCount;//������ �� ���� �� �������� ȿ������
	fin >> line;

	itemCount = line;
	itemInfos = new Item[line];//������ ����

	for (char i = 0; i < line; i++)
	{
		itemInfos[i].code = i;
		fin >> itemInfos[i].name >> cost >> effectCount;//�̸�,����,����Ʈ ����

		printf("1\n");

		itemInfos[i].cost = cost;
		itemInfos[i].effectCount = effectCount;
		itemInfos[i].effect = new unsigned short[effectCount];//����Ʈ ����
		for (char j = 0; j < effectCount; j++)//����Ʈ��
		{
			fin >> itemInfos[i].effect[j];
		}
		fin.getline(itemInfos[i].info, 100);//����
	}

	fin.close();

	return true;
}

static bool LoadMtData()//�� ������� �ʱ�ȭ
{
	ifstream fin;
	fin.open("Resources/mountain.txt");
	if (!fin.good())//������ ������ ������ �׸�
	{
		return false;
	}

	for (char i = 0; i < MAX_MT_COUNT; i++)
	{
		//�̸� , ���� ���� , ���� , ���̵�
		fin >> mtdatas[i].name >> mtdatas[i].defaultState >> mtdatas[i].height >> mtdatas[i].defficulty;
	}

	return true;
}

static bool LoadSituationData()//�����̼ǵ� �ҷ�����
{
	//������ ���� �ҷ�����
	ifstream fin;
	fin.open("Resources/situation.txt");

	if (!fin.good()) return false;//���Ͽ� ������ ����� false


	int line, effectCount;//��Ȳ�� ������ ��Ȳ�� ȿ������

	//���� ��Ȳ
	fin >> line;
	goodSituationCount = line;
	goodSituations = new Situation[line];//������ ����

	for (char i = 0; i < line; i++)
	{
		fin >> effectCount;//����Ʈ ����=
		goodSituations[i].effectCountORcode = effectCount;
		goodSituations[i].effect = new unsigned short[effectCount];//����Ʈ ����
		for (char j = 0; j < effectCount; j++)//����Ʈ��
		{
			fin >> goodSituations[i].effect[j];
		}
		fin.getline(goodSituations[i].dialog, 100);//��Ȳ����
	}

	//���� ��Ȳ
	fin >> line;
	badSituationCount = line;
	badSituations = new Situation[line];//������ ����

	for (char i = 0; i < line; i++)
	{
		fin >> effectCount;//����Ʈ ����
		badSituations[i].effectCountORcode = effectCount;
		badSituations[i].effect = new unsigned short[effectCount];//����Ʈ ����
		for (char j = 0; j < effectCount; j++)//����Ʈ��
		{
			fin >> badSituations[i].effect[j];
		}
		fin.getline(badSituations[i].dialog, 100);//��Ȳ����
	}

	fin.close();

	return true;
}

static void InitUserData()//����� ���������Ͱ� ������ �ʱ�ȭ
{
	printf("�̸��� �Է��ϼ���. : ");
	cin >> player.name;//�̸� �ޱ�

	for (char i = 0; i < INVEN_MAX; i++)//�κ� �ʱ�ȭ
		player.inven[i] = -1;

	player.pocket = new char;
	player.pocketCount = 0;
	player.money = 10000;//����!
	player.strenght = player.stamina = 100;
	player.intelligence = player.quickness = player.luck = 10;//�ɷ�ġ �ʱ�ȭ
}

static void LoadUserData()//���������� �ҷ�����
{
	ifstream fin;
	fin.open("save.txt");
	if (!fin.good())//������ ������ ������ ���� ����
	{
		printf("���̺� ���Ͽ� ������ ������ϴ�... \n\n");
		InitUserData();
		return;
	}

	//�κ��丮 �ҷ�����
	int code;
	for (char i = 0; i < INVEN_MAX; i++)
	{
		fin >> code;
		player.inven[i] = code;
	}
	//�ɷ�ġ �ҷ�����
	fin >> player.strenght >> player.stamina >> player.intelligence >> player.quickness >> player.luck;
}

static bool SaveUserData()//������ ���̺�
{
	ofstream fout;
	fout.open("save.txt");
	if (fout.good() == false)return false;

	for (char i = 0; i < INVEN_MAX; i++)//������ �����ϱ�
	{
		fout << (int)player.inven[i] << " ";
	}
	//�ɷ�ġ �����ϱ�
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

//�ΰ���


static void UseEffect(PlayerState * data, short effect, float EffectScale = 1)//������ ���
{
	short power = effect;
	//����Ʈ �����ڷ� �����κ� �����Ծ �Ŀ��� �����
	power = power << 6;
	power = power >> 6;
	power *= EffectScale;//����Ʈ ������ ���ϱ�

	if (effect & 1 << Effect::M)//�Ŀ��� ����?
		power = -power;
	if (effect & 1 << Effect::HEALTH)//ü�� ȸ��?
	{
		data->health += power;
		printf("ü���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << Effect::ENERGY)//��� ȸ��?
	{
		data->stamina += power;
		printf("����� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << Effect::INTELLIGENCE)//���� ����?
	{
		data->intelligence += power;
		printf("������ %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << Effect::QUICKNESS)//��ø ����?
	{
		data->quickness += power;
		printf("��ø���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
	if (effect & 1 << Effect::LUCK)//������?
	{
		data->luck += power;
		printf("���� %d %s�����ϴ�.\n", power, power < 0 ? "����" : "�ö�");
	}
}

static void UseItem(PlayerState * data, char index)
{
	for (char i = 0; i < itemInfos[player.pocket[index]].effectCount; i++)//������ ȿ���� ����
	{
		UseEffect(data, itemInfos[player.pocket[index]].effect[i]);
	}
}

//������//

static void InvenSort(char inven[], char left, char right)//�� ��Ʈ //������ �迭,������ ����,������ ������
{
	char i = left, j = right;//i=���� ,j=������
	char pivot = inven[(left + right) / 2];//�������� ������ �߰�
	char temp;
	do
	{
		while (inven[i] < pivot)//������ �ǹ����� (������)�۴ٸ� ��ŵ
			i++;
		while (inven[j] > pivot)//�������� �ǹ����� (������)ũ�ٸ� ��ŵ
			j--;
		if (i <= j)//������ �ɷ����� ���Ѱ� ���� ��ü
		{
			temp = inven[i];
			inven[i] = inven[j];
			inven[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);//�����̶� �������̶� ���������� �ݺ�

	//�������� ���
	if (left < j)//���� ����
		InvenSort(inven, left, j);

	if (i < right)//������ ����
		InvenSort(inven, i, right);
}

static void AddPocketItem(char code)//���Ͼ����� �߰��ϱ�
{
	//���ݴ� ū �ָӴ� �����
	player.pocketCount++;
	char * newPocket;
	newPocket = new char[player.pocketCount];

	//���ݴ� ū �ָӴϿ� ���� �ָӴ� ���빰 �ֱ�
	for (char i = 0; i < player.pocketCount - 1; i++)
		newPocket[i] = player.pocket[i];
	newPocket[player.pocketCount - 1] = code;//�� �ָӴ��� ���� �� ������ �߰�

	InvenSort(newPocket, 0, player.pocketCount - 1);//����

	//���� �ָӴ� ����� �� �ָӴϷ� �ٲ�ġ��
	delete player.pocket;
	player.pocket = newPocket;
	printf("\n\n\n%d\n\n\n", player.pocket[0]);

}

static void DeletePocketItem(char index)//���Ͼ����� �����
{
	//���� �� ���� �ָӴ� �����
	player.pocketCount--;
	char * newPocket;
	newPocket = new char[player.pocketCount];

	char j = 0;
	for (char i = 0; i < player.pocketCount; i++)//���ָӴϿ� �����ָӴ� ���빰 �ֱ�
	{
		if (index == j)//�ε����� ��ŵ
			j++;

		newPocket[i] = player.pocket[j];
		j++;
	}
	//���� �ָӴ� ����� ���ָӴϷ� �ٲ�ġ��
	delete player.pocket;
	player.pocket = newPocket;
}

static void Packing()//���α�
{
	bool x = true;
	char r;
	while (true)
	{
		while (true)
		{
			if (x)//�κ�����
				r = (SelectAblePrintInven(itemInfos, "�κ��丮", player.inven, INVEN_MAX, "����Ű�� ���ͷ� ����, ESC�� ������\n\n����", player.pocket, player.pocketCount, true));
			else//�ָӴ� ����
				r = (SelectAblePrintInven(itemInfos, "����Ű�� ���ͷ� ����, ESC�� ������\n\n����", player.pocket, player.pocketCount, "�κ��丮", player.inven, INVEN_MAX, false));
			if (r == -1)//x (��ü)
				x = !x;
			else if (r == -2)//�̽�������
				return;
			else
				break;//���ÿϷ�
		}

		if (x)//�κ����� �������� �ű��
		{
			//���Ͽ� �߰�
			AddPocketItem(player.inven[r]);


			//�κ����� ����
			player.inven[r] = -1;
			//-1(����ִ°�) �� �ڷ� ������
			char last = INVEN_MAX - 1;
			char temp;
			for (char x = 0; x < last; x++)
				if (player.inven[x] == -1)
				{
					//��ȯ
					temp = player.inven[x];
					player.inven[x] = player.inven[last];
					player.inven[last] = temp;
					while (player.inven[last] == -1) last--;//�������� ������
					x--;//�ߺ����� ���� ��������
				}
			InvenSort(player.inven, 0, last);//������ �κ� ��Ʈ

		}
		else//���Ͽ��� �κ����� ��gigi
		{
			//�κ��� �߰�
			if (player.inven[INVEN_MAX - 1] == -1)
			{
				player.inven[INVEN_MAX - 1] = player.pocket[r];

				//-1(����ִ°�) �� �ڷ� ������
				char last = INVEN_MAX - 1;
				char temp;
				for (char x = 0; x < last; x++)
					if (player.inven[x] == -1)
					{
						//��ȯ
						temp = player.inven[x];
						player.inven[x] = player.inven[last];
						player.inven[last] = temp;
						while (player.inven[last] == -1) last--;//�������� ������
						x--;//�ߺ����� ���� ��������
					}
				InvenSort(player.inven, 0, last);//������ �κ� ��Ʈ

				//���Ͽ� �ִ� ������ ����
				DeletePocketItem(r);
			}
		}
	}
}

void Shop()//����
{
	char * a = new char[itemCount];
	for (int i = 0; i < itemCount; i++)
		a[i] = i;
	while (true)
	{
		char code = SelectAblePrintInven(itemInfos, "����", a, itemCount, "����Ű�� ���ͷ� ����, ESC�� ������\n\n�κ��丮", player.inven, 20, true, true);
		if (code == -1)//(������ ��ü)
			continue;
		else if (code == -2)//�̽�������
			break;

		if (player.money >= itemInfos[code].cost && player.inven[INVEN_MAX - 1] == -1)
		{
			player.money -= itemInfos[code].cost;//�� ����
			player.inven[INVEN_MAX - 1] = code;

			//-1(����ִ°�) �� �ڷ� ������
			char last = INVEN_MAX - 1;
			char temp;
			for (char x = 0; x < last; x++)
				if (player.inven[x] == -1)
				{
					//��ȯ
					temp = player.inven[x];
					player.inven[x] = player.inven[last];
					player.inven[last] = temp;
					while (player.inven[last] == -1) last--;//�������� ������
					x--;//�ߺ����� ���� ��������
				}
			InvenSort(player.inven, 0, last);//������ �κ� ��Ʈ
		}

	}
	delete[] a;
}

//��Ȳ��

static void PlaySituation(PlayerState * state)//���߿��̼� ����
{
	Situation sit;
	float scale = 1;
	if (state->luck > rand() % 100)//���� ������
	{
		sit = goodSituations[rand() % goodSituationCount];
		printf("%s\n", sit.dialog);
		if (state->intelligence > rand() % 100)
		{
			printf("\n�׸��� ����� �Ӹ��� �Ἥ ���� ��Ȳ�� �� ���� �������.\n\n");
			scale = 2;
		}
	}
	else//���ڸ�
	{
		sit = badSituations[rand() % badSituationCount];
		printf("%s\n", sit.dialog);
		if (state->quickness > rand() % 100)//�׳��� ���� ������
		{
			printf("\n������ ������ ���ؼ� ������ ���ظ��� �Ծ���. \n\n");
			scale = 0.5;
		}
	}

	for (int i = 0; i < sit.effectCountORcode; i++)
		UseEffect(state, sit.effect[i], scale);
}

//õ�������������ۺ��۵��ư��°����ǻ���ŬS~��

void InGameSeq(char map)//�ΰ��� ������ (�׳� ���� ���ư��°�)
{
	//�ɷ�ġ �ʱ�ȭ
	PlayerState * state = new PlayerState();
	state->pos = 0;
	state->health = player.strenght;
	state->stamina = player.stamina;
	state->intelligence = player.intelligence;
	state->quickness = player.quickness;
	state->luck = player.luck;

	short mtState;//��� ȯ��
	mtState = mtdatas[map].defaultState;

	while (true)
	{
		switch (InGameMenu(map))
		{
			system("cls");
		case 1://���� ����

			printf("���� ����\n\n");
			printf("\tü�� : %d\n", state->health);
			printf("\t��� : %d\n", state->stamina);
			printf("\t���� : %d\n", state->intelligence);
			printf("\t��ø : %d\n", state->quickness);
			printf("\t� : %d\n", state->luck);
			getche();

			continue;

		case 2://������ ���
			char a = SelectAblePrintInven(itemInfos, "����Ű�� ���ͷ� ����� �������� �����ϼ���. \n�������� ESCŰ�� �����ּ���", player.pocket, player.pocketCount);
			if (a == -2) continue;
			printf("%d", a);
			UseItem(state, a);//������ ����Ʈ ���
			DeletePocketItem(a);//������ ��Ͽ��� �����
			break;
		}

		//����

		//���̵��� ���� ���� ���� �Ͼ
		if (mtdatas[map].defficulty < rand() % 100)
			PlaySituation(state);
		else
			printf("�ƹ��ϵ� ���� �����Ͽ���.\n");

		state->pos++;
		if (state->pos == mtdatas[map].height)//����
		{
			printf("\n\n���� �����ߴ�.. ����� �K�ƿ�����!\n");
			UseEffect(state, 4126);//��� 30ȸ��
		}
		else if (state->pos == mtdatas[map].height * 2)//��� ��
		{
			break;
		}
		else
		{
			printf("\n\n����� ���� %s ���� �߰����� �̾����.\n", state->pos < mtdatas[map].height ? "������" : "�Ա���");
			UseEffect(state, 5130);//��� 10�϶�
			printf("\n%s���� %d����\n", state->pos < mtdatas[map].height ? "����" : "�Ա�",
				state->pos < mtdatas[map].height ? mtdatas[map].height - state->pos : mtdatas[map].height * 2 - state->pos);
		}

		getche();
	}

	delete state;
	delete player.pocket;//�ָӴ� �б�
	player.pocket = new char[0];

	printf("\n\n����� ���� ! ������ !\n\n");
	printf("<�����е�갡�׵��ΰ�ô��������ȸ�ƽþ�����> ���� %d�� ��ŭ �������� ���Դ�.", mtdatas[map].height * mtdatas[map].defficulty * 10);
	getche();
}

void MainSeq()//���� ������
{
	while (true)
	{
		switch (MainMenu())
		{
		case 0://���ν���
			InitUserData();
			break;
		case 1://�̾��ϱ�
			LoadUserData();
			break;
		}
		while (true)
		{
			char mt = 0;
			switch (SubMainMenu(&mt))
			{
			case 0://���� ����
				InGameSeq(mt);

				break;

			case 1://�κ��丮 ����
				Packing();
				break;

			case 2://����
				Shop();
				break;

			case 3://�����ϰ� ���θ޴��� ���ư���
				if (!SaveUserData())
				{
					printf("���忡 �����Ͽ����ϴ�.");
					return;
				}
				goto ESCAPE;//goto�� ����� �ִ� ��!
			}
		}

	ESCAPE:;//����ġ��Ż��
	}
}

void EndGame()//���� ����
{
	delete[] player.pocket;
	for (char a = 0; a < itemCount; a++)//���۵�
	{
		delete[] itemInfos[a].effect;
	}
	delete[] itemInfos;

	for (char a = 0; a < goodSituationCount; a++)//���� ��Ȳ
	{
		delete[] goodSituations[a].effect;
	}
	delete[] goodSituations;

	for (char a = 0; a < badSituationCount; a++)//���� ��Ȳ
	{
		delete[] badSituations[a].effect;
	}
	delete[] badSituations;
}