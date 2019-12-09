#include "ConsoleShell.h"
#include "Console.h"
#include "Keyboard.h"
#include "Utility.h"

//Command table
SHELLCOMMANDENTRY gs_vstCommandTable[]=
{
		{"help", "Show Help", kHelp},
		{"cls", "Clear Screen", kCls},
		{"totalram", "Show Total RAM Size", kShowTotalRAMSize},
		{"strtod", "String To Decimal/Hex Convert", kStringToDecimalHexTest},
		{"shutdown", "Shutdown And Reboot OS", kShutdown},
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
				bKey = " ";
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
		if(pcCommandBuffer[iSpaceIndex]==" ")
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
			gs_vstCommandTable[i].pfFunction(pcCommandBuffer + iSpaceIndex + 1);// I don't understand iSpaceIndex is what a role.
			break;
		}
	}
	if(i >= iCount)
	{
		kPrintf("%s is not found.\n", pcCommandBuffer);
	}
}


































