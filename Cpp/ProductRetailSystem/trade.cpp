#include "stdafx.h"
#include "trade.h"
#include <fstream>
#include <time.h>
#define CAP 5    
#define TLEN 10
#define ALEN 30
using std::ofstream;
using std::ifstream;
bool trade::Buy(int ID, int count)
{
	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		if (iter->getID() == ID)
		{
			iter->UpdateStock(count);
			time_t t = time(0);
			char temp[50];
			strftime(temp, sizeof(temp), "%Y年%m月%d日%X%A", localtime(&t));
			buyRecordList.push_back(record(iter->name, count, temp));
			return true;
		}
	}
	return false;
}

bool trade::Sell(int ID, int count)
{
	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		if (iter->getID() == ID && !(iter->getStock() + count < 0))
		{
			iter->UpdateStock(-count);
			time_t t = time(0);
			char temp[50];
			strftime(temp, sizeof(temp), "%Y年%m月%d日%X%A", localtime(&t));
			sellRecordList.push_back(record(iter->name, count, temp));
			return true;
		}
	}
	return false;
}

void trade::AddNew(char name[], float buyValue, float sellValue)
{
	dataList.push_back(commodity(name, buyValue, sellValue));
}

void trade::GetIndex()
{

	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		printf("\t  商品编号: %i  商品名称: %s\n", iter->getID(), iter->getName());
	}
}

bool trade::GetInformathion(int index)
{

	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		if (iter->getID() == index)
		{
			printf("商品编号:%d\n商品名称:%s\n购入价格：%f\n出售价格：%f\n剩余：%d\n",
				index,
				iter->getName(),
				iter->getBuyValue(),
				iter->getsellValue(),
				iter->getStock());
			return true;
		}
	}
	return false;
}

void trade::save()
{
	ofstream file;
	file.open("stock.txt");
	if (!file.fail())
	{
		file << " ════════════════════════════ \n";
		file << "               ***** 商品信息 *****                       \n";
		file << " ════════════════════════════ \n";
		for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
		{
			file << "ID:" << iter->getID()
				<< "\tNAME:" << iter->getName()
				<< "\tCOST:" << iter->getBuyValue()
				<< "\tVALUE:" << iter->getsellValue()
				<< "\tSTOCK:" << iter->getStock()
				<< "\n";
		}
	}
	else
	{
		printf("记录文件创建失败");
	}
	file.close();

	file.open("sellRecord.txt");
	if (!file.fail())
	{
		file << " ════════════════════════════ \n";
		file << "               ***** 销售信息 *****                       \n";
		file << " ════════════════════════════ \n";
		for (auto iter = sellRecordList.begin(); iter != sellRecordList.end(); iter++)
		{
			file << "NAME:" << iter->name
				<< "\tTIME:" << iter->sTime
				<< "\tCOUNT:" << iter->count
				<< "\n";
		}
	}
	else
	{
		printf("销售记录文件创建失败");
	}
	file.close();

	file.open("buyRecord.txt");
	if (!file.fail())
	{
		file << " ════════════════════════════ \n";
		file << "               ***** 购入信息 *****                       \n";
		file << " ════════════════════════════ \n";
		for (auto iter = buyRecordList.begin(); iter != buyRecordList.end(); iter++)
		{
			file << "NAME:" << iter->name
				<< "\tTIME:" << iter->sTime
				<< "\tCOUNT:" << iter->count
				<< "\n";
		}
	}
	else
	{
		printf("购入记录文件创建失败");
	}

}

bool trade::init()
{
	ifstream file("stock.txt");
	if (!file.fail())
	{
		char titles[CAP][TLEN] = { "ID:","NAME:","COST:","VALUE:","STOCK:" };
		char saves[CAP][ALEN] = {};
		int tIndex = 0;
		char buf[128];
		int kinds = 0;//商品种类计数
		for (int i = 0; i < 3; i++)  //忽略标题
		{
			file.getline(buf, 128);
		}
		while (!file.eof())
		{
			char temp[TLEN] = "";//读取文件内容的字符数组
			for (int i = 0; i < TLEN && !file.eof(); i++)//for NO.1
			{
				file.get(temp[i]);
				if (strcmp(temp, titles[tIndex]) == 0)
				{
					for (int j = 0; j < ALEN && !file.eof(); j++)//for NO.2
					{
						char c;
						file.get(c);
						if (c != '\t'&&c != '\n')
						{
							saves[tIndex][j] = c;
						}
						else if (c != '\n')
						{
							if (tIndex > 4)
							{
								return false;//行参数结尾后仍然有字符存在,失效
							}
							tIndex++;
							break; //break NO.2
						}
						else
						{
							dataList.push_back(commodity(atoi(saves[0]), saves[1], atof(saves[2]), atof(saves[3]), atoi(saves[4])));
							tIndex = 0;
							kinds++;
							break; //break NO.2
						}
						if (j == ALEN - 1)
						{
							return false;//超过了参数长度,初始化失败
						}
					}//end NO.2
					break;//break NO.1
				}
				if (i == TLEN - 1)
				{
					return false; //没有匹配到参数名称,初始化失败
				}
			}//end NO.1	
		}//end While,读取结束
		commodity::kinds = kinds;
		return true;
	}
	return false;//文件不存在,初始化失败
}

void trade::getSellRecord()
{
	for (auto iter = sellRecordList.begin(); iter != sellRecordList.end(); iter++)
	{
		printf("\t出售商品名称:%s\n", iter->name);
		printf("\t交易日期:%s\n", iter->sTime);
		printf("\t出售数目:%d\n", iter->count);
		printf("\t ════════════════════════════ \n");
	}
}

void trade::getBuyRecord()
{
	for (auto iter = buyRecordList.begin(); iter != buyRecordList.end(); iter++)
	{

		printf("\t购入商品名称:%s\n", iter->name);
		printf("\t交易日期:%s\n", iter->sTime);
		printf("\t购入数目:%d\n", iter->count);
		printf("\t ════════════════════════════ \n");
	}
}

