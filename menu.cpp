#include "stdafx.h"
#include "menu.h"
#include "conio.h"


menu::menu()
{
	getCurrencyList();
	getPlatformData();
	getCurrencyData();
}

void menu::getCurrencyList()
{
	char* fileName = "d:/MyFiles/curlist.bin", buff[8];
	
	FILE *f = fopen(fileName, "rb");
	if (f == NULL)
	{
		std::cout << "File " << fileName << " not found." << std::endl;
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	sizeCurList = ftell(f) / sizeof(buff);
	currencyList = new char*[sizeCurList];
	fseek(f, 0, SEEK_SET);

	for (int i = 0; i < sizeCurList; i++)
	{
		currencyList[i] = new char[sizeof(buff)];
		fread(currencyList[i], sizeof(buff), 1, f);
		fseek(f, (i+1)*sizeof(buff), SEEK_SET);
	}

	fclose(f);
}

void menu::getPlatformData()
{
	char* fileName = "d:/MyFiles/platforms.bin";

	FILE *f = fopen(fileName, "rb");
	if (f == NULL)
	{
		std::cout << "File " << fileName << " not found." << std::endl;
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	sizeArrPlatData = ftell(f) / sizeof(platformData);
	pArrayPlatData = new platformData[sizeArrPlatData];
	fseek(f, 0, SEEK_SET);
	fread(pArrayPlatData, sizeof(platformData), sizeArrPlatData, f);
	fclose(f);
}

void menu::getCurrencyData()
{
	char* fileName = "d:/MyFiles/curData.bin";

	FILE *f = fopen(fileName, "rb");
	if (f == NULL)
	{
		std::cout << "File " << fileName << " not found." << std::endl;
		exit(0);
	}
	fseek(f, 0, SEEK_END);
	sizeArrCurData = ftell(f) / sizeof(currencyData);
	pCurData = new currencyData;
	fseek(f, 0, SEEK_SET);
	fread(pCurData, sizeof(currencyData), sizeArrCurData, f);
	fclose(f);
}

void menu::startTrade()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	int choice;

	std::cout << "Choose platform:" << std::endl;
	for (int i = 0; i < sizeArrPlatData; i++)
		std::cout << i + 1 << " - " << pArrayPlatData[i].name << std::endl;
	choice = _getch()-'0';

	std::cout << "Downloading platform..." << std::endl;

	platform optionPlatform(pArrayPlatData[choice-1]);
	HRESULT hr = optionPlatform.start();

	if (FAILED(hr))
		exit(0);
	system("cls");

	std::cout << "Choose currency:" << std::endl;
	for(int i = 0; i < sizeCurList; i++)
		std::cout << i + 1 << " - " << currencyList[i]<< std::endl;
	choice = _getch()-'0';
	std::string str(currencyList[choice - 1]);
	std::wstring currencyPair = s2ws(str);

	system("cls");
	std::cout << "Searching for buttons..." << std::endl;

	currency cur(*pCurData,currencyPair);
	cur.recievePrices();

	int leap, leap2, diff;
	bool cont;
	char answer;

	if (!optionPlatform.search)
		try
		{
			optionPlatform.findElements(optionPlatform.getRootElement());
		}
		catch (const char *str)
		{
			std::cout << str << std::endl;
		}

	do
	{
		if (optionPlatform.search)
			try
			{
				optionPlatform.setButtonsToNull();
				optionPlatform.findElements(optionPlatform.getRootElement());
			}
			catch (const char *str)
			{
				std::cout << str << std::endl;
			}

		std::cout << "Enter the size of the first leap: ";
		std::cin >> leap;
		std::cout << "Enter the size of the second leap: ";
		std::cin >> leap2;

		cont = true;

		while (cont)
		{
			diff = cur.getLastLeap();
			if (diff > leap)
			{
				for (int i = 0; i < 3; i++)
					if (cur.getLastLeap() > leap2)
					{
						hr = optionPlatform.clickHigh();
						cont = false;
					}
			}
			else if (diff < -leap)
				for (int i = 0; i < 3; i++)
					if (cur.getLastLeap() < -leap2)
					{
						hr = optionPlatform.clickLow();
						cont = false;
					}
		}
		std::cout << "\aDo you want to continue?(y/n)";
		std::cin >> answer;
	} while (answer == 'y');

	CoUninitialize();
}

