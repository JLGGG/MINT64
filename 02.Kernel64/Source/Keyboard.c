#include "Types.h"
#include "AssemblyUtility.h"
#include "Keyboard.h"

////////////////////////////////////////////
// keyboard controller and related function for keyboard control
////////////////////////////////////////////
// If exist received data in output buffer, state return
BOOL kIsOutputBufferFull(void)
{
	if(kInPortByte(0x64) & 0x01)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL kIsInputBufferFull(void)
{
	if(kInPortByte(0x64) & 0x02)
	{
		return TRUE;
	}
	return FALSE;
}

//keyboard activate
BOOL kActivateKeyboard(void)
{
	int i;
	int j;

	//keyboard device driver active
	kOutPortByte(0x64, 0xAE);

	for(i=0; i<0xFFFF; i++)
	{
		if(kIsInputBufferFull() == FALSE)
		{
			break;
		}
	}

	//keyboard device active
	kOutPortByte(0x60, 0xF4);

	for(j=0; j<100; j++)
	{
		for(i=0; i<0xFFFF; i++)
		{
			if(kIsOutputBufferFull() == TRUE)
			{
				break;
			}

		}

		if(kInPortByte(0x60) == 0xFA)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BYTE kGetKeyboardScanCode(void)
{
	while(kIsOutputBufferFull()==FALSE) { ; }
	return kInPortByte(0x60);
}

BOOL kChangeKeyboardLED(BOOL bCapsLockOn, BOOL bNumLockOn, BOOL bScrollLockOn)
{
	int i, j;

	//transmit LED change command to keyboard and then wait until processing command
	for(i=0; i<0xFFFF; i++)
	{
		//if output buffer empty, command transmit
		if(kIsInputBufferFull() == FALSE)
		{
			break;
		}
	}

	kOutPortByte(0x60, 0xED);
	for(i=0; i<0xFFFF; i++)
	{
		if(kIsInputBufferFull() == FALSE) { break; }
	}

	for(j=0; j<100; j++)
	{
		for(i=0; i<0xFFFF; i++)
		{
			if(kIsOutputBufferFull() == TRUE) { break; }
		}

		if(kInPortByte(0x60) == 0xFA) { break; }
	}
	if(j>=100)
	{
		return FALSE;
	}

	kOutPortByte(0x60, (bCapsLockOn << 2) | (bNumLockOn << 1) | bScrollLockOn);
	for(i=0; i<0xFFFF; i++)
	{
		if(kIsInputBufferFull()==FALSE) { break; }
	}

	for(j=0; j<100; j++)
	{
		for(i=0; i<0xFFFF; i++)
		{
			if(kIsOutputBufferFull()==TRUE)
			{
				break;
			}
		}

		if(kInPortByte(0x60) == 0xFA){break;}
	}
	if(j>=100){return FALSE;}

	return TRUE;
}

//activate A20 gate
void kEnableA20Gate(void)
{
	BYTE bOutputPortData;
	int i;

	kOutPortByte(0x64, 0xD0);

	for(i=0; i<0xFFFF; i++)
	{
		if(kIsOutputBufferFull()==TRUE)
		{
			break;
		}
	}

	bOutputPortData=kInPortByte(0x60);

	//set up A20 gate bit
	bOutputPortData |= 0x01;

	for(i=0; i<0xFFFF; i++)
	{
		if(kIsInputBufferFull()==FALSE)
		{
			break;
		}
	}

	kOutPortByte(0x64, 0xD1);

	kOutPortByte(0x60, bOutputPortData);
}


//reset a processor..
void kReboot(void)
{
	int i;

	for(i=0; i<0xFFFF; i++)
	{
		if(kIsInputBufferFull()==FALSE)
		{
			break;
		}
	}

	kOutPortByte(0x64, 0xD1);

	//processor reset
	kOutPortByte(0x60, 0x00);

	while(1)
	{
		;
	}
}

//////////////////////////////////////////////////
//function which change scan code to ASCII code
//////////////////////////////////////////////////

//keyboard manager to manage keyboard status
static KEYBOARDMANAGER gs_stKeyboardManager={0,};

static KEYMAPPINGENTRY gs_vstKeyMappingTable[KEY_MAPPINGTABLEMAXCOUNT]=
{
		/* 0 */ { KEY_NONE , KEY_NONE},
		/* 1 */ { KEY_ESC  , KEY_ESC},
};




































































