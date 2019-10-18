[BITS 64]

SECTION .text

global kInPortByte, kOutPortByte, kLoadGDTR, kLoadTR, kLoadIDTR

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
