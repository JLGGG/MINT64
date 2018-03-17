/*
 * EntryPoint.s
 *
 *  Created on: Mar 16, 2018
 *      Author: Lim JunSu(JLGGG)
 */

[BITS 64]

SECTION .text  ;define text section

;Import Main
extern Main

::::::::::::::::::::::::::::::::::::::::::
:  code area
::::::::::::::::::::::::::::::::::::::::::
START:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	;stack set up 1MB size, range 0x600000~ 0x6FFFFF
	mov ss, ax
	mov rsp, 0x6FFFF8
	mov rbp, 0x6FFFF8

	call Main

	jmp $
