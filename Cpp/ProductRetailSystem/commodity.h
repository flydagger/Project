#include <string.h>
class commodity
{
private:
	int static kinds;//��Ʒ����
	int ID; //��Ʒ���
	int stock;//�����
	float buyValue;//�����ļ۸�
	float sellValue;//�����ļ۸�
	char name[30];//��Ʒ����

public:
	int getID();//��ȡID
	int getStock();//��ȡ���
	float getBuyValue();//��ȡ������
	float getsellValue();//��ȡ������
	char* getName();//��ȡ��Ʒ����
	void UpdateStock(int n);;  //�ı�����
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
