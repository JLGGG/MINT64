/*
 * Main.c
 *
 *  Created on: Mar 16, 2018
 *      Author: Lim JunSu(JLGGG)
 */


#include "Types.h"
#include "Keyboard.h"
#include "Descriptor.h"
#include "PIC.h"
#include "AssemblyUtility.h"
#include "PrintString.h"


void Main(void)
{
	char vcTemp[2]={0,};
	BYTE bFlags;
	BYTE bTemp;
	int i=0;

	kPrintString(0,10, "Switch To IA-32e Mode Success~!!");
	kPrintString(0,11, "IA-32e C Language Kernel Start..............[Pass]");
	kPrintString(0,12, "GDT Initialize And Switch For IA-32e Mode...[    ]");
	kInitializeGDTTableAndTSS();
	kLoadGDTR(GDTR_STARTADDRESS);
	kPrintString(45,12,"Pass");

	kPrintString(0,13, "TSS Segment Load............................[    ]");
	kLoadTR(GDT_TSSSEGMENT);
	kPrintString(45,13,"Pass");

	kPrintString(0,14, "IDT Initialize..............................[    ]");
	kInitializeIDTTables();
	kLoadIDTR(IDTR_STARTADDRESS);
	kPrintString(45,14,"Pass");

	kPrintString(0,15, "Keyboard Activate...........................[    ]");


	if(kActivateKeyboard()==TRUE)
	{
		kPrintString(45, 15, "Pass");
		kChangeKeyboardLED(FALSE, FALSE, FALSE);
	}
	else
	{
		kPrintString(45, 15, "Fail");
		while(1);
	}

	kPrintString(0,16,"PIC Controller And Interrupt Initialize.....[    ]");
	//PIC controller initialize and all interrupt activation
	kInitializePIC();
	kMaskPICInterrupt(0);
	kEnableInterrupt();
	kPrintString(45,16,"Pass");

	while(1)
	{
		if(kIsOutputBufferFull()==TRUE)
		{
			bTemp=kGetKeyboardScanCode();

			if(kConvertScanCodeToASCIICode(bTemp, &(vcTemp[0]), &bFlags)==TRUE)
			{
				if(bFlags & KEY_FLAGS_DOWN)
				{
					kPrintString(i++, 17, vcTemp);

					if(vcTemp[0]=='0')
					{
						bTemp=bTemp/0; //divide error test
					}
				}
			}
		}
	}

}


