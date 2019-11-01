#include "PIC.h"

void kInitializePIC(void)
{
	//initialize master PIC controller
	//ICW1(port 0x20), IC4 bit(bit 0) = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x11);

	//ICW2(port 0x21), interrupt vector(0x20)
	kOutPortByte(PIC_MASTER_PORT2, PIC_IRQSTARTVECTOR);

	//ICW3(port 0x21), connection position in slave PIC controller(express bit)
	kOutPortByte(PIC_MASTER_PORT2, 0x04);

	//ICW4(port 0x21), uPM bit(bit 0) = 1
	kOutPortByte(PIC_MASTER_PORT2, 0x01)

	//initialize slave PIC controller
	//ICW1(port 0xA0), IC4 bit(bit 0) = 1
	kOutPortByte(PIC_SLAVE_PORT1, 0x11);

	//ICW2(port 0xA1), interrupt vector(0x20 + 8)
	kOutPortByte(PIC_SLAVE_PORT2, PIC_IRQSTARTVECTOR + 8);

	//ICW3(port 0xA1), connection position in master PIC controller(express integer)
	kOutPortByte(PIC_SLAVE_PORT2, 0x02);

	//ICW4(port 0xA1), uPM bit(bit 0) = 1
	kOutPortByte(PIC_SLAVE_PORT2, 0x01);
}

void kMaskPICInterrupt(WORD wIRQBitmask)
{
	//set up IMR in master PIC controller
	//OCW1(port 0x21), IRQ 0~ IRQ 7
	kOutPortByte(PIC_MASTER_PORT2, (BYTE) wIRQBitmask);

	//set up IMR in slave PIC controller
	//OCW1(port 0xA1), IRQ 8~ IRQ 15
	kOutPortByte(PIC_SLAVE_PORT2, (BYTE) (wIRQBitmask >> 8));
}

// send EOI(meaning : interrupt handle completion)
void kSendEOIToPIC(int iIRQNumber)
{
	//OCW2(port 0x20), EOI bit(bit 5) = 1
	kOutPortByte(PIC_MASTER_PORT1, 0x20);

	if(iIRQNumber >= 8)
	{
		//OCW2(port 0xA0), EOI bit(bit 5) = 1
		kOutPortByte(PIC_SLAVE_PORT1, 0x20);
	}
}

