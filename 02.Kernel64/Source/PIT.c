#include "PIT.h"

void kInitializePIT(WORD wCount, BOOL bPeriodic)
{
	kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_ONCE);

	//Set to mode 2 if it is a timer that repeats at regular intervals.
	if(bPeriodic == TRUE)
	{
		kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_PERIODIC);
	}

	//Set the counter initial value in order of LSB -> MSB on the counter 0.
	kOutPortByte(PIT_PORT_COUNTER0, wCount);
	kOutPortByte(PIT_PORT_COUNTER0, wCount >> 8);
}

//Return current value of the counter 0.
WORD kReadCounter0(void)
{
	BYTE bHighByte, bLowByte;
	WORD wTemp = 0;

	kOutPortByte(PIT_PORT_CONTROL, PIT_COUNTER0_LATCH);

	//Read the counter value in order of LSB -> MSB on the counter 0.
	bLowByte = kInPortByte(PIT_PORT_COUNTER0);
	bHighByte = kInPortByte(PIT_PORT_COUNTER0);

	//Returns the value read by sum of 16bit.
	wTemp = bHighByte;
	wTemp = (wTemp << 8) | bLowByte;
	return wTemp;
}

//Set counter 0 directly to wait for a period of time or longer.
//Recalling the function changes the PIT controller's setting, so you must reset the PIT controller later.
//For accurate measurement, it is recommended to disable interrupt before using the function.
//It can be measured up to about 50ms.
void kWaitUsingDirectPIT(WORD wCount)
{
	WORD wLastCounter0;
	WORD wCurrentCounter0;

	//Set the PIT controller over counting from 0 to 0xFFFF repeatedly.
	kInitializePIT(0, TRUE);

	//Wait from now until more than wCount increase.
	wLastCounter0 = kReadCounter0();
	while(1)
	{
		//Return the value of current counter 0.
		wCurrentCounter0 = kReadCounter0();
		if(((wLastCounter0 - wCurrentCounter0) & 0xFFFF) >= wCount)
		{
			break;
		}
	}
}
