#pragma once
#include "platform.h"
#include "currency.h"
#include <string>

class menu
{
public:
	menu();
	void getCurrencyList();
	void getPlatformData();
	void getCurrencyData();
	void startTrade();
private:
	char** currencyList;
	platformData* pArrayPlatData;
	currencyData* pCurData;
	int sizeArrPlatData;
	int sizeArrCurData;
	int sizeCurList;
};