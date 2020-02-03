[BITS 64]

SECTION .text

global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR
global kEnableInterrupt, kDisableInterrupt, kReadRFLAGS
global kReadTSC

kInPortByte:
	push rdx

	mov rdx, rdi
	mov rax, 0
	in al, dx

	pop rdx
	ret

kOutPortByte:
	push rdx
	push rax

	mov rdx, rdi
	mov rax, rsi
	out dx, al

	pop rax
	pop rdx
	ret

;Set up GDT table in GDTR register
; PARAM: address of GDT table data structure
kLoadGDTR:
	lgdt[rdi]
	ret

;Set up TSS segment descriptor in TR register
; PARAM: TSS segment descriptor offset
kLoadTR:
	ltr di
	ret

;Set up IDT table in IDTR register
; PARAM: address of IDT table data structure
kLoadIDTR:
	lidt[rdi]
	ret

;activate interrupt
; PARAM: no
kEnableInterrupt:
	sti
	ret

;deactivate interrupt
; PARAM: no
kDisableInterrupt:
	cli
	ret

;read RFLAGS register and then return
; PARAM: no
kReadRFLAGS:
	pushfq
	pop rax

	ret

;Return time stamp counter by reading
; PARAM: no
kReadTSC:
	push rdx

	rdtsc		; Read a time stamp counter and save it to RDX:RAX

	shl rdx, 32	; upper 32bit TSC value on RDX register and lower 32bit TSC value on RAX register
	or rax, rdx	; in OR operation save a 64bit TSC value to RAX register.

	pop rdx
	ret

