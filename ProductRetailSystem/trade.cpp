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
			strftime(temp, sizeof(temp), "%Y��%m��%d��%X%A", localtime(&t));
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
			strftime(temp, sizeof(temp), "%Y��%m��%d��%X%A", localtime(&t));
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
		printf("\t  ��Ʒ���: %i  ��Ʒ����: %s\n", iter->getID(), iter->getName());
	}
}

bool trade::GetInformathion(int index)
{

	for (auto iter = dataList.begin(); iter != dataList.end(); iter++)
	{
		if (iter->getID() == index)
		{
			printf("��Ʒ���:%d\n��Ʒ����:%s\n����۸�%f\n���ۼ۸�%f\nʣ�ࣺ%d\n",
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
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
		file << "               ***** ��Ʒ��Ϣ *****                       \n";
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
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
		printf("��¼�ļ�����ʧ��");
	}
	file.close();

	file.open("sellRecord.txt");
	if (!file.fail())
	{
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
		file << "               ***** ������Ϣ *****                       \n";
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
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
		printf("���ۼ�¼�ļ�����ʧ��");
	}
	file.close();

	file.open("buyRecord.txt");
	if (!file.fail())
	{
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
		file << "               ***** ������Ϣ *****                       \n";
		file << " �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n";
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
		printf("�����¼�ļ�����ʧ��");
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
		int kinds = 0;//��Ʒ�������
		for (int i = 0; i < 3; i++)  //���Ա���
		{
			file.getline(buf, 128);
		}
		while (!file.eof())
		{
			char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
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
								return false;//�в�����β����Ȼ���ַ�����,ʧЧ
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
							return false;//�����˲�������,��ʼ��ʧ��
						}
					}//end NO.2
					break;//break NO.1
				}
				if (i == TLEN - 1)
				{
					return false; //û��ƥ�䵽��������,��ʼ��ʧ��
				}
			}//end NO.1	
		}//end While,��ȡ����
		commodity::kinds = kinds;
		return true;
	}
	return false;//�ļ�������,��ʼ��ʧ��
}

void trade::getSellRecord()
{
	for (auto iter = sellRecordList.begin(); iter != sellRecordList.end(); iter++)
	{
		printf("\t������Ʒ����:%s\n", iter->name);
		printf("\t��������:%s\n", iter->sTime);
		printf("\t������Ŀ:%d\n", iter->count);
		printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
	}
}

void trade::getBuyRecord()
{
	for (auto iter = buyRecordList.begin(); iter != buyRecordList.end(); iter++)
	{

		printf("\t������Ʒ����:%s\n", iter->name);
		printf("\t��������:%s\n", iter->sTime);
		printf("\t������Ŀ:%d\n", iter->count);
		printf("\t �T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T \n");
	}
}

