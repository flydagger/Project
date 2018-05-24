#include "stdafx.h"
#include "trade.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

void ToBuy(int& ID,int& count)
{
	cout<<"�����빺����Ʒ�ı��"<<endl;
    cin>>ID;
	getchar();
	cout<<"�����빺����Ʒ������:"<<endl;
	cin>>count;
	getchar();
}

void ToSell(int& ID, int& count)
{
	cout << "������������Ʒ�ı��" << endl;
	cin >> ID;
	getchar();
	cout << "������������Ʒ������:" << endl;
	cin >> count;
	getchar();
}

void operate()
{
	printf("�����������");
	getchar();
	system("cls");

}

void drawIndex()
{
	system("cls");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
	printf("\t               ***** ��ƷĿ¼ *****                       \n");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");

}

void drawLine()
{
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
}

void DrawMainMenu()
{
	printf("\t�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[\n");
	printf("\t�U                 ��ӭʹ������ϵͳ                     �U\n");
	printf("\t�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g\n");
	printf("\t�U                 1 -   ������Ʒ                       �U\n");
	printf("\t�U                 2 -   ������Ʒ                       �U\n");
	printf("\t�U                 3 -   �����Ʒ                       �U\n");
	printf("\t�U                 4 -   �鿴��Ʒ��Ϣ                   �U\n");
	printf("\t�U                 5 -   �鿴�ɹ���¼                   �U\n");
	printf("\t�U                 6 -   �鿴���ۼ�¼                   �U\n");
	printf("\t�U                 7 -   �˳�                           �U\n");
	printf("\t�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n");
}

void drawBuyRecord()
{
	system("cls");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
	printf("\t               ***** �ɹ���¼ *****                         \n");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
}

void drawSellRecord()
{
	system("cls");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
	printf("\t               ***** ���ۼ�¼ *****                         \n");
	printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
}


int main(int argc, _TCHAR* argv[])
{
	trade myTrade;
	if (!myTrade.init())
	{
		myTrade = trade();
	}
	bool quitFlag = false;
	while (!quitFlag)
	{
		DrawMainMenu();
		printf("����������ѡ��:");
		int selection;
		cin >> selection;
		getchar();
		int ID;
		int count;
		switch (selection)
		{
		case 1:
			drawIndex();
			myTrade.GetIndex();
			drawLine();;
			ToBuy(ID, count);
			if (myTrade.Buy(ID, count))
			{
				system("cls");
				printf("�����ɹ�,");
			}
			else
			{
				system("cls");
				printf("������������,");
			}
			operate();
			break;
		case 2:
			drawIndex();
			myTrade.GetIndex();
			drawLine();
			ToSell(ID, count);
			if (myTrade.Sell(ID, count))
			{
				system("cls");
				printf("�����ɹ�,");
			}
			else
			{
				system("cls");
				printf("������������,");

			}
			operate();
			break;
		case 3:
			char name[30];
			float value;
			float cost;
			system("cls");
			cout << "��������Ʒ������" << endl;
			cin >> name;
			getchar();
			cout << "�����빺��۸�" << endl;
			cin >> cost;
			getchar();
			cout << "��������ۼ۸�" << endl;
			cin >> value;
			getchar();
			myTrade.AddNew(name, cost, value);
			system("cls");
			printf("�����ɹ�,");
			operate();
			break;
		case 4:
			drawIndex();
			myTrade.GetIndex();
			drawLine();
			cout << "��������Ʒ���:";
			int index;
			cin >> index;
			getchar();
			system("cls");
			if (!myTrade.GetInformathion(index))
			{

				cout << "��Ч����Ʒ��ţ�";
				operate();
			}
			else {
				operate();

			}
			break;
		case 5:
			drawBuyRecord();
			myTrade.getBuyRecord();
			operate();
			break;
		case 6:
			drawSellRecord();
			myTrade.getSellRecord();
			operate();
			break;
		case 7:
			quitFlag = true;
			break;
		default:
			system("cls");
			printf("��Ч��ѡ�");
			operate();

		}
	}
	myTrade.save();
	return 0;
}
