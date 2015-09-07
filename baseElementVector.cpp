#include "stdafx.h"
#include "baseElementVector.h"

baseElementVector::baseElementVector(int n):curIndex(0),size(n)
{
	pArray = new baseElement*[size];
}

baseElementVector::~baseElementVector()
{
	for (int i = 0; i < size; i++)
		delete pArray[i];

	delete[] pArray;
}

void baseElementVector :: pushBack(baseElement* elem)
{
	pArray[curIndex++] = elem;
}

baseElement* baseElementVector::Back()
{
	return (curIndex == 0) ? pArray[curIndex] : pArray[curIndex-1];
}