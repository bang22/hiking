#include "hikingSim.h"



int main()
{
	if (!GameInit())//�ʱ�ȭ
	{
		printf("�������α׶��� �ڷῡ ������ �������ϴ�.\n\n");
		getche();
	}
	else
		MainSeq();//���� ��

	EndGame();//�����Ҵ� ����
	return 0;
}

