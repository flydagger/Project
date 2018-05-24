#include <string.h>
class commodity
{
private:
	int static kinds;//商品种类
	int ID; //货品编号
	int stock;//库存量
	float buyValue;//进货的价格
	float sellValue;//卖出的价格
	char name[30];//商品名称

public:
	int getID();//获取ID
	int getStock();//获取库存
	float getBuyValue();//获取进货价
	float getsellValue();//获取卖出价
	char* getName();//获取商品名称
	void UpdateStock(int n);;  //改变库存量
	friend class trade;

	commodity(char name[], float buyValue, float sellValue)
	{
		kinds++;
		ID = kinds;
		this->stock = 0;
		this->buyValue = buyValue;
		this->sellValue = sellValue;
		strcpy(this->name, name);
	}

	commodity(int ID, char name[], float buyValue, float sellValue, int stock)
	{
		kinds++;
		this->ID = ID;
		this->stock = stock;
		this->buyValue = buyValue;
		this->sellValue = sellValue;
		strcpy(this->name, name);
	}
};
