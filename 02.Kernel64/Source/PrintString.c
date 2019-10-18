#include "PrintString.h"

void kPrintString(int iX, int iY, const char* pcString)
{
	CHARACTOR* pstScreen = (CHARACTOR*) 0xB8000;
	int i;

	pstScreen += (iY * 80) + iX;

	for(i=0;pcString[i]!=0;i++)
	{
		pstScreen[i].bCharactor=pcString[i];
	}
}
