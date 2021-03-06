[ORG 0x00] // Set code start address to 0x00
[BITS 16] // Code below is set to 16-bit code.

SECTION .text // Define text segment.

jmp 0x07C0:START // Move to START label, copying 0x07C0 to cs segment register.

TOTALSECTORCOUNT: dw 2 // Size of MINT64 OS image, except bootloader

KERNEL32SECTORCOUNT: dw 0x02


START:
	mov ax, 0x07C0 // Conversions the bootloader' start address(0x07C0) as a segment register value.
	mov ds, ax // Set to DS segment register.
	mov ax, 0xB800 // Conversions the video memory' start address(0xB800) as a segment register value.
	mov es, ax // Set to ES segment register.
	// Set stack to 64KB size in 0x0000:0000 to 0x0000:FFFF areas.
	mov ax, 0x0000 // Conversions the stack segment' start address(0x0000) as a segment register value.
	mov ss, ax // Set to SS segment register.
	mov sp, 0xFFFE // Set address of SP register to 0xFFFE.(SP = Stack Point)
	mov bp, 0xFFFE // Set address of BP register to 0xFFFE.(BP = Base Point)

	// Clear all screens, set property values to green
	mov si, 0 // Initialize SI register(string original index register).

.SCREENCLEARLOOP: // Loop for erasing screen.
	mov byte[es:si], 0 // Delete a character by copying 0 to the address where the character in the video memory resides.
	mov byte[es:si+1], 0x0A // Copy 0x0A to the address where the attribute in the video memory resides.

	add si, 2 // Because you have set characters and properties, go to the following location.

	cmp si, 80*25*2
	jl .SCREENCLEARLOOP


	push MESSAGE1
	push 0
	push 0
	call PRINTMESSAGE
	add sp, 6

	push IMAGELOADINGMESSAGE
	push 1
	push 0
	call PRINTMESSAGE
	add sp, 6

RESETDISK:
	mov ax, 0
	mov dl, 0
	int 0x13
	jc HANDLEDISKERROR

	mov si, 0x1000
	mov es, si
	mov bx, 0x0000

	mov di, word[TOTALSECTORCOUNT]

READDATA:
	cmp di, 0
	je READEND
	sub di, 0x1

	;BIOS READ FUNCTION
	mov ah, 0x02
	mov al, 0x1
	mov ch, byte[TRACKNUMBER]
	mov cl, byte[SECTORNUMBER]
	mov dh, byte[HEADNUMBER]
	mov dl, 0x00
	int 0x13
	jc HANDLEDISKERROR

	add si, 0x0020
	mov es, si

	mov al, byte[SECTORNUMBER]
	add al, 0x01
	mov byte[SECTORNUMBER], al
	cmp al, 19
	jl READDATA

	xor byte[HEADNUMBER], 0x01
	mov byte[SECTORNUMBER], 0x01

	cmp byte[HEADNUMBER], 0x00
	jne READDATA

	add byte[TRACKNUMBER], 0x01
	jmp READDATA
READEND:

	push LOADINGCOMPLETEMESSAGE
	push 1
	push 20
	call PRINTMESSAGE
	add sp, 6

	;Run the loaded virtual os image
	jmp 0x1000:0x0000

HANDLEDISKERROR:
	push DISKERRORMESSAGE
	push 1
	push 20
	call PRINTMESSAGE

	jmp $

PRINTMESSAGE:
	push bp
	mov bp, sp

	push es
	push si
	push di
	push ax
	push cx
	push dx

	mov ax, 0xB800
	mov es, ax

	mov ax, word[bp+6]
	mov si, 160
	mul si
	mov di, ax

	mov ax, word[bp+4]
	mov si, 2
	mul si
	add di, ax

	mov si, word[bp+8]

.MESSAGELOOP:
	mov cl, byte[si]

	cmp cl, 0
	je .MESSAGEEND

	mov byte[es:di], cl

	add si, 1
	add di, 2

	jmp .MESSAGELOOP

.MESSAGEEND:
	pop dx
	pop cx
	pop ax
	pop di
	pop si
	pop es
	pop bp
	ret


MESSAGE1: db 'MINT64 OS Boot Loader Start~!', 0
DISKERRORMESSAGE: db 'DISK Error...!', 0
IMAGELOADINGMESSAGE: db 'OS Image Loading...', 0
LOADINGCOMPLETEMESSAGE: db 'Complete...!', 0

SECTORNUMBER db 0x02
HEADNUMBER db 0x00
TRACKNUMBER db 0x00


times 510-($-$$) db 0x00

db 0x55
db 0xAA
