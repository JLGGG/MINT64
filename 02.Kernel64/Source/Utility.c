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

//Returns the amount of RAM
QWORD kGetTotalRAMSize(void)
{
	return gs_qwTotalRAMMBSize;
}

//Internal implementation of atoi()
long kAToI(const char* pcBuffer, int iRadix)
{
	long lReturn;

	switch(iRadix)
	{
	case 16:
		lReturn = kHexStringToQword(pcBuffer);
		break;
	case 10:
	default:
		lReturn = kDecimalStringToLong(pcBuffer);
		break;
	}
	return lReturn;
}

//Convert hex string to QWORD
QWORD kHexStringToQword(const char* pcBuffer)
{
	QWORD qwValue = 0;
	int i;

	//Return in turn by rotating the string.
	for(i=0; pcBuffer[i]!='\0'; i++)
	{
		qwValue *= 16;
		if(('A'<=pcBuffer[i]) && (pcBuffer[i]<='Z'))
		{
			qwValue += (pcBuffer[i] - 'A') + 10;
		}
		else if(('a'<=pcBuffer[i]) && (pcBuffer[i]<='z'))
		{
			qwValue += (pcBuffer[i] - 'a') + 10;
		}
		else
		{
			qwValue += pcBuffer[i] - '0';
		}
	}
	return qwValue;
}

//Convert decimal string to long
long kDecimalStringToLong(const char* pcBuffer)
{
	long lValue = 0;
	int i;

	if(pcBuffer[0] == '-')
	{
		i = 1;
	}
	else
	{
		i = 0;
	}

	//Return in turn by rotating the string.
	for( ; pcBuffer[i]!='\0'; i++)
	{
		lValue *= 10;
		lValue += pcBuffer[i] - '0';
	}

	//If negative number, add minus sign.
	if(pcBuffer[0] == '-')
	{
		lValue = -lValue;
	}
	return lValue;
}

//Internal implementation of itoa()
int kIToA(long lValue, char* pcBuffer, int iRadix)
{
	int iReturn;

	switch(iRadix)
	{
		//hex
	case 16:
		iReturn = kHexToString(lValue, pcBuffer);
		break;

	case 10:
	default:
		iReturn = kDecimalToString(lValue, pcBuffer);
		break;
	}

	return iReturn;
}

//Convert hex value to string.
int kHexToString(QWORD qwValue, char* pcBuffer)
{
	QWORD i;
	QWORD qwCurrentValue;

	//Process as soon as 0 is delivered.
	if(qwValue == 0)
	{
		pcBuffer[0] = '0';
		pcBuffer[1] = '\0';
		return 1;
	}

	for(i=0; qwValue > 0; i++)
	{
		qwCurrentValue = qwValue % 16;
		if(qwCurrentValue >= 10)
		{
			pcBuffer[i] = 'A' + (qwCurrentValue - 10);
		}
		else
		{
			pcBuffer[i] = '0' + qwCurrentValue;
		}

		qwValue = qwValue / 16;
	}
	pcBuffer[i] = '\0';

	kReverseString(pcBuffer);
	return i;
}

//Convert decimal value to string
int kDecimalToString(long lValue, char* pcBuffer)
{
	long i;

	if(lValue == 0)
	{
		pcBuffer[0] = '0';
		pcBuffer[1] = '\0';
		return 1;
	}

	if(lValue < 0)
	{
		i = 1;
		pcBuffer[0] = '-';
		lValue = -lValue;
	}
	else
	{
		i = 0;
	}

	for( ; lValue > 0 ; i++)
	{
		pcBuffer[i] = '0' + lValue % 10;
		lValue = lValue / 10;
	}
	pcBuffer[i] = '\0';

	if(pcBuffer[0] == '-')
	{
		kReverseString(&(pcBuffer[1]));
	}
	else
	{
		kReverseString(pcBuffer);
	}

	return i;
}



