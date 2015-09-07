#pragma once
#include <cpprest\http_client.h>

struct baseElement
{
	utility::string_t time;
	double price;
	int leap;
};

class baseElementVector 
{
private:
	baseElement** pArray;
	int size;
	int curIndex;
public:
	baseElementVector(int);
	~baseElementVector();
	void pushBack(baseElement*);
	baseElement* Back();
};