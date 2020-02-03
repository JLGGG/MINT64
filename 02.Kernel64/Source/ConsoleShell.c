#include "ConsoleShell.h"
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"
#include "PIT.h"
#include "PIC.h"
#include "AssemblyUtility.h"

//Command table
SHELLCOMMANDENTRY gs_vstCommandTable[]=
{
		{"help", "Show Help", kHelp},
		{"cls", "Clear Screen", kCls},
		{"totalram", "Show Total RAM Size", kShowTotalRAMSize},
		{"strtod", "String To Decimal/Hex Convert", kStringToDecimalHexTest},
		{"shutdown", "Shutdown And Reboot OS", kShutdown},
		{"settimer", "Set PIT Controller Counter0, ex)settimer 10(ms) 1(periodic)", kSetTimer},
		{"wait", "Wait ms Using PIT, ex)wait 100(ms)", kWaitUsingPIT},
		{"rdtsc", "Read Time Stamp Counter", kReadTimeStampCounter},
		{"cpuspeed", "Measure Processor Speed", kMeasureProcessorSpeed},
		{"date", "Show Date And Time", kShowDateAndTime},
};

//Main loop of shell
void kStartConsoleShell(void)
{
	char vcCommandBuffer[CONSOLESHELL_MAXCOMMANDBUFFERCOUNT];
	int iCommandBufferIndex = 0;
	BYTE bKey;
	int iCursorX, iCursorY;

	//output prompt
	kPrintf("%s", CONSOLESHELL_PROMPTMESSAGE);

	while(1)
	{
		bKey = kGetCh();
		if(bKey == KEY_BACKSPACE)
		{
			if(iCommandBufferIndex > 0)
			{
				kGetCursor(&iCursorX, &iCursorY);
				kPrintStringXY(iCursorX - 1, iCursorY, " ");
				kSetCursor(iCursorX - 1, iCursorY);
				iCommandBufferIndex--;
			}
		}
		else if(bKey == KEY_ENTER)
		{
			kPrintf("\n");

			if(iCommandBufferIndex > 0)
			{
				//execute a command in the buffer
				vcCommandBuffer[iCommandBufferIndex]='\0';
				kExecuteCommand(vcCommandBuffer);
			}

			kPrintf("%s", CONSOLESHELL_PROMPTMESSAGE);
			kMemSet(vcCommandBuffer, '\0', CONSOLESHELL_MAXCOMMANDBUFFERCOUNT);
			iCommandBufferIndex = 0;
		}
		else if((bKey == KEY_LSHIFT) || (bKey == KEY_RSHIFT) ||
				(bKey == KEY_CAPSLOCK) || (bKey == KEY_NUMLOCK) ||
				(bKey == KEY_SCROLLLOCK))
		{
			;
		}
		else
		{
			if(bKey == KEY_TAB)
			{
				bKey = ' ';
			}

			if(iCommandBufferIndex < CONSOLESHELL_MAXCOMMANDBUFFERCOUNT)
			{
				vcCommandBuffer[iCommandBufferIndex++]=bKey;
				kPrintf("%c", bKey);
			}
		}
	}
}

void kExecuteCommand(const char* pcCommandBuffer)
{
	int i, iSpaceIndex;
	int iCommandBufferLength, iCommandLength;
	int iCount;

	//extract space-separated commands.
	iCommandBufferLength = kStrLen(pcCommandBuffer);
	for(iSpaceIndex=0; iSpaceIndex < iCommandBufferLength; iSpaceIndex++)
	{
		if(pcCommandBuffer[iSpaceIndex]==' ')
		{
			break;
		}
	}

	iCount = sizeof(gs_vstCommandTable) / sizeof(SHELLCOMMANDENTRY);
	for(i=0; i<iCount; i++)
	{
		iCommandLength = kStrLen(gs_vstCommandTable[i].pcCommand);
		if((iCommandLength == iSpaceIndex) && (kMemCmp(gs_vstCommandTable[i].pcCommand, pcCommandBuffer, iSpaceIndex) == 0))
		{
			gs_vstCommandTable[i].pfFunction(pcCommandBuffer + iSpaceIndex + 1);// pass on the next factor of the command.
			break;
		}
	}
	if(i >= iCount)
	{
		kPrintf("%s is not found.\n", pcCommandBuffer);
	}
}

//initialize parameter structure.
void kInitializeParameter(PARAMETERLIST* pstList, const char* pcParameter)
{
	pstList->pcBuffer = pcParameter;
	pstList->iLength = kStrLen(pcParameter);
	pstList->iCurrentPosition = 0;
}

//return the content and length of a parameter separated by a space.
int kGetNextParameter(PARAMETERLIST* pstList, char* pcParameter)
{
	int i;
	int iLength;

	//return if no more parameters.
	if(pstList->iLength <= pstList->iCurrentPosition)
	{
		return 0;
	}

	//search for space while moving the length of the buffer.
	for(i=pstList->iCurrentPosition; i<pstList->iLength; i++)
	{
		if(pstList->pcBuffer[i]==' ')
		{
			break;
		}
	}

	kMemCpy(pcParameter, pstList->pcBuffer + pstList->iCurrentPosition, i);
	iLength = i - pstList->iCurrentPosition;
	pcParameter[iLength]='\0';

	pstList->iCurrentPosition += iLength+1;
	return iLength;
}

//==========================================================================================
//The code that handles the command.
//==========================================================================================
void kHelp(const char* pcCommandBuffer)
{
	int i;
	int iCount;
	int iCursorX, iCursorY;
	int iLength, iMaxCommandLength = 0;

	kPrintf("=====================================================\n");
	kPrintf("                 MINT64 Shell Help                   \n");
	kPrintf("=====================================================\n");

	iCount = sizeof(gs_vstCommandTable)/sizeof(SHELLCOMMANDENTRY);

	for(i=0;i<iCount;i++)
	{
		iLength = kStrLen(gs_vstCommandTable[i].pcCommand);
		if(iLength > iMaxCommandLength)
		{
			iMaxCommandLength = iLength;
		}
	}

	for(i=0;i<iCount;i++)
	{
		kPrintf("%s", gs_vstCommandTable[i].pcCommand);
		kGetCursor(&iCursorX, &iCursorY);
		kSetCursor(iMaxCommandLength, iCursorY);
		kPrintf(" - %s\n", gs_vstCommandTable[i].pcHelp);
	}
}

void kCls(void)
{
	kClearScreen();
	kSetCursor(0, 1);
}

void kShowTotalRAMSize(const char* pcParameterBuffer)
{
	kPrintf("Total RAM Size = %d MB\n", kGetTotalRAMSize());
}

void kStringToDecimalHexTest(const char* pcParameterBuffer)
{
	char vcParameter[100];
	int iLength = 0;
	PARAMETERLIST stList;
	int iCount = 0;
	long lValue;

	kInitializeParameter(&stList, pcParameterBuffer);

	while(1)
	{
		iLength = kGetNextParameter(&stList, vcParameter);
		if(iLength == 0)
		{
			break;
		}

		kPrintf("Param %d = %s, Length = %d ", iCount + 1, vcParameter, iLength);

		if(kMemCmp(vcParameter, "0x", 2)==0)
		{
			lValue = kAToI(vcParameter + 2, 16);
			kPrintf("HEX Value = %q\n", lValue);
		}
		else
		{
			lValue = kAToI(vcParameter, 10);
			kPrintf("Decimal Value = %d\n", lValue);
		}

		iCount++;
	}
}

void kShutdown(void)
{
	kPrintf("System Shutdown Start.....\n");

	kPrintf("Press Any Key To Reboot PC...");
	kGetCh();
	kReboot();
}

// Set up counter 0 of the PIT controller
void kSetTimer(const char* pcParameterBuffer)
{
	char vcParameter[100];
	PARAMETERLIST stList;
	long lValue;
	BOOL bPeriodic;

	kInitializeParameter(&stList, pcParameterBuffer);

	//extract millisecond
	if(kGetNextParameter(&stList, vcParameter) == 0)
	{
		kPrintf("ex)settimer 10(ms) 1(periodic)\n");
		return ;
	}
	lValue = kAToI(vcParameter, 10);

	//extract periodic
	if(kGetNextParameter(&stList, vcParameter) == 0)
	{
		kPrintf("ex)settimer 10(ms) 1(periodic)\n");
		return ;
	}
	bPeriodic = kAToI(vcParameter, 10);

	kInitializePIT(MSTOCOUNT(lValue), bPeriodic);
	kPrintf("Time = %d ms, Periodic = %d Change Complete\n", lValue, bPeriodic);
}

//Wait ms using PIT controller directly.
void kWaitUsingPIT(const char* pcParameterBuffer)
{
	char vcParameter[100];
	int iLength;
	PARAMETERLIST stList;
	long lMillisecond;
	int i;

	kInitializeParameter(&stList, pcParameterBuffer);
	if(kGetNextParameter(&stList, vcParameter) == 0)
	{
		kPrintf("ex)wait 100(ms)\n");
		return ;
	}

	lMillisecond = kAToI(pcParameterBuffer, 10);
	kPrintf("%d ms Sleep Start...\n", lMillisecond);

	//disable interrupt and measure time from the PIT controller.
	kDisableInterrupt();
	for(i=0; i<lMillisecond/30; i++)
	{
		kWaitUsingDirectPIT(MSTOCOUNT(30));
	}
	kWaitUsingDirectPIT(MSTOCOUNT(lMillisecond % 30));
	kEnableInterrupt();
	kPrintf("%d ms Sleep Complete\n", lMillisecond);

	//recover timer.
	kInitializePIT(MSTOCOUNT(1), TRUE);
}

//Read a time stamp counter.
void kReadTimeStampCounter(const char* pcParameterBuffer)
{
	QWORD qwTSC;

	qwTSC = kReadTSC();
	kPrintf("Time Stamp Counter = %q\n", qwTSC);
}

//Measure the speed of the processor.
void kMeasureProcessorSpeed(const char* pcParameterBuffer)
{
	int i;
	QWORD qwLastTSC, qwTotalTSC = 0;

	kPrintf("Now Measuring.");

	//Indirectly measure the speed of the processor using changed time stamp counter for 10 second.
	kDisableInterrupt();
	for(i=0; i<200; i++)
	{
		qwLastTSC = kReadTSC();
		kWaitUsingDirectPIT(MSTOCOUNT(50));
		qwTotalTSC += kReadTSC() - qwLastTSC;

		kPrintf(".");
	}
	//Recover the timer.
	kInitializePIT(MSTOCOUNT(1), TRUE);
	kEnableInterrupt();

	kPrintf("\nCPU Speed = %d MHz\n", qwTotalTSC / 10 / 1000 / 1000);
}

void kShowDateAndTime(const char* pcParameterBuffer)
{
	BYTE bSecond, bMinute, bHour;
	BYTE bDayOfWeek, bDayOfMonth, bMonth;
	WORD wYear;

	kReadRTCTime(&bHour, &bMinute, &bSecond);
	kReadRTCDate(&wYear, &bMonth, &bDayOfMonth, &bDayOfWeek);

	kPrintf("Date: %d/%d/%d %s, ", wYear, bMonth, bDayOfMonth, kConvertDayOfWeekToString(bDayOfWeek));
	kPrintf("Time: %d:%d:%d\n", bHour, bMinute, bSecond);
}
































