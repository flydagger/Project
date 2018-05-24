#include "stdafx.h"
#include <list>
#include "commodity.h"
using std::list;
class trade {
	struct record
	{
		char name[30];//��Ʒ����
		int count;//��������
		char sTime[70];//����ʱ��
		record(char* name, int count, char* time)
		{
			strcpy(this->name, name);
			this->count = count;
			strcpy(sTime, time);
		}
	};

	//��Ա����
private:
	list<commodity> dataList;//��Ʒ��������
	list<record> sellRecordList;
	list<record> buyRecordList;

	//��Ա����
public:
	bool GetInformathion(int index);//��ȡ�������Ʒ��Ϣ
	void GetIndex();//��ȡ�������ƷĿ¼
	bool init();//�ӱ����ļ���ȡ��Ʒ��Ϣ
	void save();//����Ʒ��Ϣ���浽�����ļ���
	bool Buy(int ID, int count);//������Ʒ�Ĳ��������ݼ��
	bool Sell(int n, int ID);//������Ʒ�Ĳ��������ݼ��
	void AddNew(char name[], float buyValue, float sellValue);//����µ���Ʒ
	/*�����Ʒ�Ĺ�������ۼ�¼*/
	void getSellRecord();
	void getBuyRecord();
};
