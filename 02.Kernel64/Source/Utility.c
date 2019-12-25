#include "Utility.h"
#include "AssemblyUtility.h"
#include <stdarg.h>

void kMemSet(void* pvDestination, BYTE bData, int iSize)
{
	int i;

	for(i=0; i<iSize; i++)
	{
		((char*)pvDestination)[i]=bData;
	}
}

int kMemCpy(void* pvDestination, const void* pvSource, int iSize)
{
	int i;

	for(i=0; i<iSize; i++)
	{
		((char*)pvDestination)[i]=((char*)pvSource)[i];
	}

	return iSize;
}

int kMemCmp(const void* pvDestination, const void* pvSource, int iSize)
{
	int i;
	char cTemp;

	for(i=0; i<iSize; i++)
	{
		cTemp=((char*)pvDestination)[i]-((char*)pvSource)[i];
		if(cTemp!=0)
		{
			return (int) cTemp;
		}
	}
	return 0;
}

BOOL kSetInterruptFlag(BOOL bEnableInterrupt)
{
	QWORD qwRFLAGS;

	qwRFLAGS = kReadRFLAGS();
	if(bEnableInterrupt == TRUE)
	{
		kEnableInterrupt();
	}
	else
	{
		kDisableInterrupt();
	}

	if(qwRFLAGS & 0x0200)
	{
		return TRUE;
	}
	return FALSE;
}

int kStrLen(const char* pcBuffer)
{
	int i;

	for(i=0; ;i++)
	{
		if(pcBuffer[i]=='\0')
		{
			break;
		}
	}
	return i;
}

//Total size of the RAM(MB unit).
static QWORD gs_qwTotalRAMMBSize = 0;

//check the ram size from a location greater than 64MB
//Require only one call during the initial boot process.
void kCheckTotalRAMSize(void)
{
	DWORD* pdwCurrentAddress;
	DWORD dwPreviousValue;

	//Starting the examination in 64MB to 4MB increments.
	pdwCurrentAddress = (DWORD*)0x4000000;
	while(1)
	{
		//Store values that were previously in memory.
		dwPreviousValue = *pdwCurrentAddress;
		*pdwCurrentAddress = 0x12345678;
		if(*pdwCurrentAddress != 0x12345678)
		{
			break;
		}
		*pdwCurrentAddress = dwPreviousValue;
		//Move to next 4MB location.(??)
		pdwCurrentAddress += (0x400000 / 4);
	}

	//Checked addresses divided by 1MB and calculated in MB unit.
	gs_qwTotalRAMMBSize = (QWORD)pdwCurrentAddress / 0x100000;
}


