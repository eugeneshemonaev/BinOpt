//#pragma once
#include "Mshtml.h"

struct platformData
{
	char name[20];
	char url[100];
	char buttons[3][100];
	bool reSearch;
};

class platform
{
public:
	platform(const platformData&);
	const bool search;
	HRESULT start();
	HRESULT clickLow();
	HRESULT clickHigh();
	HRESULT findElements(IHTMLElement*);
	void	setButtonsToNull();
	IHTMLElement* getRootElement();
	friend void classNameCompare(IHTMLElement*, platform&);
protected:
	IHTMLDocument3* doc;
	IHTMLElement *Low, *High, *Buy;
	BSTR *elements;
	char *url;
	
};

