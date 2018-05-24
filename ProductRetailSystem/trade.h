#include "stdafx.h"
#include <list>
#include "commodity.h"
using std::list;
class trade {
	struct record
	{
		char name[30];//商品名称
		int count;//交易数量
		char sTime[70];//交易时间
		record(char* name, int count, char* time)
		{
			strcpy(this->name, name);
			this->count = count;
			strcpy(sTime, time);
		}
	};

	//成员变量
private:
	list<commodity> dataList;//商品数据链表
	list<record> sellRecordList;
	list<record> buyRecordList;

	//成员函数
public:
	bool GetInformathion(int index);//获取并输出商品信息
	void GetIndex();//获取并输出商品目录
	bool init();//从本地文件获取商品信息
	void save();//将商品信息保存到本地文件中
	bool Buy(int ID, int count);//购买商品的操作与数据检查
	bool Sell(int n, int ID);//购买商品的操作与数据检查
	void AddNew(char name[], float buyValue, float sellValue);//添加新的商品
	/*获得商品的购入和销售记录*/
	void getSellRecord();
	void getBuyRecord();
};
