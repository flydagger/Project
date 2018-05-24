#include "stdafx.h"
#include "trade.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

void ToBuy(int& ID,int& count)
{
	cout<<"请输入购买商品的编号"<<endl;
    cin>>ID;
	getchar();
	cout<<"请输入购买商品的数量:"<<endl;
	cin>>count;
	getchar();
}

void ToSell(int& ID, int& count)
{
	cout << "请输入卖出商品的编号" << endl;
	cin >> ID;
	getchar();
	cout << "请输入卖出商品的数量:" << endl;
	cin >> count;
	getchar();
}

void operate()
{
	printf("按任意键继续");
	getchar();
	system("cls");

}

void drawIndex()
{
	system("cls");
	printf("\t ════════════════════════════ \n");
	printf("\t               ***** 商品目录 *****                       \n");
	printf("\t ════════════════════════════ \n");

}

void drawLine()
{
	printf("\t ════════════════════════════ \n");
}

void DrawMainMenu()
{
	printf("\t╔═══════════════════════════╗\n");
	printf("\t║                 欢迎使用销售系统                     ║\n");
	printf("\t╠═══════════════════════════╣\n");
	printf("\t║                 1 -   购进商品                       ║\n");
	printf("\t║                 2 -   卖出商品                       ║\n");
	printf("\t║                 3 -   添加新品                       ║\n");
	printf("\t║                 4 -   查看商品信息                   ║\n");
	printf("\t║                 5 -   查看采购记录                   ║\n");
	printf("\t║                 6 -   查看销售记录                   ║\n");
	printf("\t║                 7 -   退出                           ║\n");
	printf("\t╚═══════════════════════════╝\n");
}

void drawBuyRecord()
{
	system("cls");
	printf("\t ════════════════════════════ \n");
	printf("\t               ***** 采购记录 *****                         \n");
	printf("\t ════════════════════════════ \n");
}

void drawSellRecord()
{
	system("cls");
	printf("\t ════════════════════════════ \n");
	printf("\t               ***** 销售记录 *****                         \n");
	printf("\t ════════════════════════════ \n");
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
		printf("请输入您的选项:");
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
				printf("操作成功,");
			}
			else
			{
				system("cls");
				printf("您的输入有误,");
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
				printf("操作成功,");
			}
			else
			{
				system("cls");
				printf("您的输入有误,");

			}
			operate();
			break;
		case 3:
			char name[30];
			float value;
			float cost;
			system("cls");
			cout << "请输入新品的名称" << endl;
			cin >> name;
			getchar();
			cout << "请输入购入价格" << endl;
			cin >> cost;
			getchar();
			cout << "请输入出售价格" << endl;
			cin >> value;
			getchar();
			myTrade.AddNew(name, cost, value);
			system("cls");
			printf("操作成功,");
			operate();
			break;
		case 4:
			drawIndex();
			myTrade.GetIndex();
			drawLine();
			cout << "请输入商品编号:";
			int index;
			cin >> index;
			getchar();
			system("cls");
			if (!myTrade.GetInformathion(index))
			{

				cout << "无效的商品编号，";
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
			printf("无效的选项，");
			operate();

		}
	}
	myTrade.save();
	return 0;
}
