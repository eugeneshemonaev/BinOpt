#pragma once
#include "windows.h"
#include "baseElementVector.h"


struct currencyData
{
	char url[100];
	char token[100];
};

class currency
{
public:
	currency(const currencyData&, std::wstring);
	void recievePrices();
	int getLastLeap();

	friend DWORD WINAPI startThread(void *pV);

private:
	std::wstring url;
	std::wstring token;
	baseElementVector container;

};

std::wstring s2ws(const std::string& str);