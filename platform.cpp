#include "stdafx.h"
#include "platform.h"
#include "comutil.h"
#import "progid:InternetExplorer.Application"
#include <iostream>

platform::platform(const platformData& platData):search(platData.reSearch)
{
	setButtonsToNull();
	url = new char[sizeof(platData.url)];
	memcpy(url, platData.url, sizeof(platData.url));
	//BSTR* classNames = new BSTR[3];
	elements = new BSTR[3];
	elements[0] = _com_util::ConvertStringToBSTR(platData.buttons[0]);
	elements[1] = _com_util::ConvertStringToBSTR(platData.buttons[1]);
	elements[2] = _com_util::ConvertStringToBSTR(platData.buttons[2]);
	//elements = classNames;
}

HRESULT platform::start()
{
	SHDocVw::IWebBrowser2Ptr ptrIE(_T("InternetExplorer.Application"));

	ptrIE->Visible = true;

	HRESULT hr = ptrIE->Navigate(url);
	if (FAILED(hr))
		return hr;

	int i = 0;
	// ждем пока загрузится страница по url
	while (ptrIE->Busy)
		i++;

	hr = ptrIE->Document->QueryInterface(IID_IHTMLDocument3, (void**)&doc);

	return hr;

}

void platform::setButtonsToNull()
{
	Low = High = Buy = NULL;
}

IHTMLElement* platform::getRootElement()
{
	IHTMLElement *root;
	doc->get_documentElement(&root);
	return root;
}

HRESULT platform::findElements(IHTMLElement* elem)
{
	IDispatch * pDisp;
	IHTMLElement * childElem;
	IHTMLElementCollection *elemColChildren;
	HRESULT hr = elem->get_children(&pDisp);

	hr = pDisp->QueryInterface(IID_IHTMLElementCollection, (void **)&elemColChildren);
	pDisp->Release();
	long length;
	_variant_t index;
	elemColChildren->get_length(&length);
	if (length == 0)
	{
		classNameCompare(elem, *this);
		return S_OK;
	}
	else
	{
		for (long i = 0; i < length; i++)
		{
			index = i;
			hr = elemColChildren->item(index, index, &pDisp);
			if (pDisp == NULL)
				continue;
			hr = pDisp->QueryInterface(IID_IHTMLElement, (void **)&childElem);
			pDisp->Release();
			findElements(childElem);
			if (Low == childElem || High == childElem || Buy == childElem)
				continue;
			childElem->Release();
		}
		classNameCompare(elem, *this);
		elemColChildren->Release();
	}

	return S_OK;
}

void classNameCompare(IHTMLElement* elem, platform& object)
{
	BSTR foundClassName;
	elem->get_className(&foundClassName);
	if (VarBstrCmp(foundClassName, object.elements[0], LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE) == VARCMP_EQ)
		object.Low = elem;
	else if (VarBstrCmp(foundClassName, object.elements[1], LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE) == VARCMP_EQ)
		object.High = elem; 
	else if (VarBstrCmp(foundClassName, object.elements[2], LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE) == VARCMP_EQ)
		object.Buy = elem;

	if (object.Low != NULL && object.High != NULL && object.Buy != NULL)
		throw "All buttons are found\a";
}

HRESULT platform::clickLow()
{
	return Low->click();
	//return Buy->click();
}

HRESULT platform::clickHigh()
{
	return High->click();
	//return Buy->click();
}
