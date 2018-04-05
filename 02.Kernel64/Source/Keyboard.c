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




































































