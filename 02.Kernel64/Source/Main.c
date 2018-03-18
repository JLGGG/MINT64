/*
 * Main.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Lim JunSu(JLGGG)
 */


#include "Types.h"

void kPrintString(int iX, int iY, const char* pcString);

void Main(void)
{
	kPrintString(0,10, "Switch To IA-32e Mode Success~!!");
	kPrintString(0,11, "IA-32e C Language Kernel Start..............[Pass]");

}

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

