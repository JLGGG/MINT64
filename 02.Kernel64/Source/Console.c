#include <stdarg.h>
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"
#include "AssemblyUtility.h"
#include "Types.h"

//Data structure for managing information on console
CONSOLEMANAGER gs_stConsoleManager = {0,};

//initialize console
void kInitializeConsole(int iX, int iY)
{
	kMemSet(&gs_stConsoleManager, 0, sizeof(gs_stConsoleManager));

	//Set cursor position
	kSetCursor(iX, iY);
}

void kSetCursor(int iX, int iY)
{
	int iLinearValue;

	//calculate the position of the cursor
	iLinearValue = iY * CONSOLE_WIDTH + iX;

	//select the upper cursor position register by sending 0x0E to the CRTC control address register
	kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_UPPERCURSOR);
	//print the high-order bytes of the cursor to the CRTC control data register
	kOutPortByte(VGA_PORT_DATA, iLinearValue >> 8);

	//select the lower cursor position register by sending 0x0F to the CRTC control address register
	kOutPortByte(VGA_PORT_INDEX, VGA_INDEX_LOWERCURSOR);
	//print the low-order bytes of the cursor to the CRTC control data register
	kOutPortByte(VGA_PORT_DATA, iLinearValue & 0xFF);

	//update where to print characters.
	gs_stConsoleManager.iCurrentPrintOffset = iLinearValue;
}

//return position of the current cursor
void kGetCursor(int* piX, int* piY)
{
	*piX = gs_stConsoleManager.iCurrentPrintOffset % CONSOLE_WIDTH;
	*piY = gs_stConsoleManager.iCurrentPrintOffset / CONSOLE_WIDTH;
}

//internal implementation of the printf function
void kPrintf(const char* pcFormatString, ...)
{
	va_list ap;
	char vcBuffer[1024];
	int iNextPrintOffset;

	//process vsprintf() using variable factor list.
	va_start(ap, pcFormatString);
	kVSPrintf(vcBuffer, pcFormatString, ap);
	va_end(ap);

	//Print a format string to the screen.
	iNextPrintOffset = kConsolePrintString(vcBuffer);

	//Update position of the cursor.
	kSetCursor(iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH);
}

//Outputs a string that contains characters such as \n, \t and then returns
//the next location on the screen.
int kConsolePrintString(const char* pcBuffer)
{
	CHARACTER* pstScreen = (CHARACTER*) CONSOLE_VIDEOMEMORYADDRESS;
	int i, j;
	int iLength;
	int iPrintOffset;

	//Save where you want to print the string.
	iPrintOffset = gs_stConsoleManager.iCurrentPrintOffset;

	//Outputs the length of the string to the screen.
	iLength = kStrLen(pcBuffer);
	for(i=0 ; i<iLength ; i++)
	{
		//Line-up processing
		if(pcBuffer[i] == '\n')
		{
			//Move the position to output to the column of 80.
			//Add the number of strings remaining for the current line and
			//position it as the next line.
			iPrintOffset += (CONSOLE_WIDTH - (iPrintOffset % CONSOLE_WIDTH));
		}
		//tab processing
		else if(pcBuffer[i] == '\t')
		{
			//Move the position to output to the column of 8.
			iPrintOffset += (8-(iPrintOffset % 8));
		}
		//Output general string
		else
		{
			//Set characters and properties in video memory and print character
			//and then move the next position for output position.
			pstScreen[iPrintOffset].bCharacter = pcBuffer[i];
			pstScreen[iPrintOffset].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
			iPrintOffset++;
		}

		//Scroll when the position to be output is outside the maximum value.
		if(iPrintOffset >= (CONSOLE_HEIGHT*CONSOLE_WIDTH))
		{
			kMemCpy(CONSOLE_VIDEOMEMORYADDRESS, CONSOLE_VIDEOMEMORYADDRESS + CONSOLE_WIDTH * sizeof(CHARACTER),
					(CONSOLE_HEIGHT - 1) * CONSOLE_WIDTH * sizeof(CHARACTER));

			//The last line is filled with spaces.
			for(j=(CONSOLE_HEIGHT - 1)*(CONSOLE_WIDTH);
					j<(CONSOLE_HEIGHT*CONSOLE_WIDTH); j++)
			{
				pstScreen[j].bCharacter = ' ';
				pstScreen[j].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
			}

			iPrintOffset = (CONSOLE_HEIGHT - 1)*CONSOLE_WIDTH;
		}
	}
	return iPrintOffset;
}

//delete the whole screen.
void kClearScreen(void)
{
	CHARACTER* pstScreen = (CHARACTER*) CONSOLE_VIDEOMEMORYADDRESS;
	int i;

	for(i=0; i<CONSOLE_WIDTH*CONSOLE_HEIGHT; i++)
	{
		pstScreen[i].bCharacter = ' ';
		pstScreen[i].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
	}

	//Move the cursor to the top of the screen.
	kSetCursor(0, 0);
}

//implementation the getch().
BYTE kGetCh(void)
{
	KEYDATA stData;

	while(1)
	{
		while(kGetKeyFromKeyQueue(&stData)==FALSE)
		{
			;
		}

		if(stData.bFlags & KEY_FLAGS_DOWN)
		{
			return stData.bASCIICode;
		}
	}
}

//Output string to the x, y position.
void kPrintStringXY(int iX, int iY, const char* pcString)
{
	CHARACTER* pstScreen = (CHARACTER*)CONSOLE_VIDEOMEMORYADDRESS;
	int i;

	pstScreen += (iY*80) + iX;
	for(i=0; pcString[i]!=0; i++)
	{
		pstScreen[i].bCharacter = pcString[i];
		pstScreen[i].bAttribute = CONSOLE_DEFAULTTEXTCOLOR;
	}
}




