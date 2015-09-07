#include "stdafx.h"
#include "currency.h"

using namespace web;                       
using namespace web::http;                 
using namespace web::http::client;          

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

currency::currency(const currencyData& cData, std::wstring curPair): container(1000000)
{
	url = s2ws(cData.url) + curPair;
	token = s2ws(cData.token);
}

DWORD WINAPI startThread(void *pV)
{
	currency *pObj = (currency*)pV;
	web::json::value body;
	double price, oldPrice = 0, diff;
	utility::string_t time;
	http_client client(pObj->url);

	http_request request(methods::GET);
	request.headers().add(U("Content-type"), U("application/x-www-form-urlencoded"));
	request.headers().add(U("Connection"), U("Keep-Alive"));
	request.headers().add(U("Authorization"), pObj->token);

	while (true)
	{
		client.request(request).then([&](http_response response)
		{
			if (response.status_code() == status_codes::OK)
			{
				body = response.extract_json().get();
				auto iterArray = body.as_object().cbegin()->second.as_array().begin()._Ptr->as_object().cbegin()._Ptr;

				//oldPrice = (iterArray[0].second.as_double() + iterArray[1].second.as_double()) / 2;
				price = iterArray[0].second.as_double();
				diff = (price - oldPrice) * 100000;
				diff += diff > 0 ? 0.5 : -0.5;
				if (price != oldPrice)
				{
					pObj->container.pushBack(new baseElement{ iterArray[3].second.as_string(), price, (int)(diff) });
					oldPrice = price;
				}
			}
		}).wait();
	}
}

void currency::recievePrices()
{
	CreateThread(0, 0, startThread, this, 0, 0);
}


int currency::getLastLeap()
{
	return container.Back()->leap;
}