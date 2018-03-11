#include "Types.h"
#include "Page.h"

void kPrintString(int iX, int iY, const char* pcString);
BOOL kInitializeKernel64Area(void);
BOOL kIsMemoryEnough(void);

void Main(void)
{
	DWORD i;

	kPrintString(0,3, "C Language Kernel Start....................[Pass]");

	kPrintString(0,4, "Minimum Memory Size Check..................[    ]");
	if(kIsMemoryEnough()==FALSE)
	{
		kPrintString(44,4,"Fail");
		kPrintString(0,5, "Not Enough Memory!! MINT64 OS Requires Over 64Mbyte Memory~!!");
		while(1);
	}
	else
	{
		kPrintString(44,4,"Pass");
	}

	kPrintString(0,5, "IA-32e Kernel Area Initialize..............[    ]");
	if(kInitializeKernel64Area()==FALSE)
	{
		kPrintString(44,5, "Fail");
		kPrintString(0,6, "Kernel Area Initialization Fail~!!");
		while(1);
	}
	kPrintString(44,5,"Pass");

	kPrintString(0,6, "IA-32e Page Tables Initialize..............[    ]");
	kInitializePageTables();
	kPrintString(44,6,"Pass");

	while(1);
}

void kPrintString(int iX, int iY, const char* pcString)
{
	CHARACTOR* pstScreen = (CHARACTOR*) 0xB8000;
	int i;

	pstScreen += (iY*80) + iX;
	for(i=0; pcString[i] != 0; i++)
	{
		pstScreen[i].bCharactor = pcString[i];
	}
}

BOOL kInitializeKernel64Area(void)
{
	DWORD* pdwCurrentAddress;

	pdwCurrentAddress = (DWORD*) 0x100000;

	while((DWORD)pdwCurrentAddress < 0x600000)
	{
		*pdwCurrentAddress = 0x00;

		if(*pdwCurrentAddress != 0)
		{
			return FALSE;
		}

		pdwCurrentAddress++;

	}

	return TRUE;
}

BOOL kIsMemoryEnough(void)
{
	DWORD* pdwCurrentAddress;

	pdwCurrentAddress = (DWORD*) 0x100000;

	while((DWORD) pdwCurrentAddress < 0x4000000)
	{
		*pdwCurrentAddress = 0x12345678;

		if(*pdwCurrentAddress != 0x12345678)
		{
			return FALSE;

		}

		// Question!!
		// Why divide in 4??
		pdwCurrentAddress += (0x100000 / 4);

	}
	return TRUE;
}

