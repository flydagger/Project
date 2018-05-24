#include "stdafx.h"
#include "commodity.h"
//≥ı ºªØID
int commodity::kinds  = 0;//static member must be initialised outside of declaration
void commodity::UpdateStock(int n)
{
	stock += n;
}

int commodity::getStock()
{
	return stock;
}

int commodity::getID()
{
	return ID;
}

float commodity::getBuyValue()
{
	return buyValue;
}

float commodity::getsellValue()
{
	return sellValue;
}

char* commodity::getName()
{
	return name;
}
