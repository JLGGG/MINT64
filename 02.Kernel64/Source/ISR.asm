[BITS 64]

SECTION .text
;Import
extern kCommonExceptionHandler, kCommonInterruptHandler, kKeyboardHandler
;Export
;ISR for exception handling
global kISRDivideError, kISRDebug, kISRNMI, kISRBreakPoint, kISROverflow
global kISRBoundRangeExceeded, kISRInvalidOpcode, kISRDeviceNotAvailable, kISRDoubleFault
global kISRCoprocessorSegmentOverrun, kISRInvalidTSS, kISRSegmentNotPresent
global kISRStackSegmentFault, kISRGeneralProtection, kISRPageFault, kISR15
global kISRFPUError, kISRAlignmentCheck, kISRMachineCheck, kISRSIMDError, kISRETCException

;ISR for interrupt handling
global kISRTimer, kISRKeyboard, kISRSlavePIC, kISRSerial2, kISRSerial1, kISRParallel2
global kISRFloppy, kISRParallel1, kISRRTC, kISRReserved, kISRNotUsed1, kISRNotUsed2
global kISRMouse, kISRCoprocessor, kISRHDD1, kISRHDD2, kISRETCInterrupt

;macro which save the context and change the selector
%macro KSAVECONTEXT 0
	; insert to stack from RBP register to GS segment selector
	push rbp
	mov rbp, rsp
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov ax, ds
	push rax
	mov ax, es
	push rax
	push fs
	push gs

	;exchange segment selector
	mov ax, 0x10 ; 0x10 refers to the kernel data segment
	mov ds, ax
	mov es, ax
	mov gs, ax
	mov fs, ax
%endmacro

%macro KLOADCONTEXT 0
	pop gs
	pop fs
	pop rax
	mov es, ax
	pop rax
	mov ds, ax

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	pop rbp
%endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	exception handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; #0, Divide Error ISR
kISRDivideError:
	KSAVECONTEXT

	mov rdi, 0
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #1, Debug ISR
kISRDebug:
	KSAVECONTEXT

	mov rdi, 1
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #2, NMI ISR
kISRNMI:
	KSAVECONTEXT

	mov rdi, 2
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #3, BreakPoint ISR
kISRBreakPoint:
	KSAVECONTEXT

	mov rdi, 3
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #4, Overflow ISR
kISROverflow:
	KSAVECONTEXT

	mov rdi, 4
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #5, Bound Range Exceeded ISR
kISRBoundRangeExceeded:
	KSAVECONTEXT

	mov rdi, 5
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #6, Invalid Opcode ISR
kISRInvalidOpcode:
	KSAVECONTEXT

	mov rdi, 6
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #7, Device Not Available ISR
kISRDeviceNotAvailable:
	KSAVECONTEXT

	mov rdi, 7
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #8, Double Fault ISR
kISRDoubleFault:
	KSAVECONTEXT

	mov rdi, 8
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #9, Coprocessor Segment Overrun ISR
kISRCoprocessorSegmentOverrun:
	KSAVECONTEXT

	mov rdi, 9
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #10, Invalid TSS ISR
kISRInvalidTSS:
	KSAVECONTEXT

	mov rdi, 10
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #11, Segment Not Present ISR
kISRSegmentNotPresent:
	KSAVECONTEXT

	mov rdi, 11
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #12, Stack Segement Fault ISR
kISRStackSegmentFault:
	KSAVECONTEXT

	mov rdi, 12
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #13, General Protection ISR
kISRGeneralProtection:
	KSAVECONTEXT

	mov rdi, 13
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #14, Page Fault ISR
kISRPageFault:
	KSAVECONTEXT

	mov rdi, 14
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #15, Reserved ISR
kISR15:
	KSAVECONTEXT

	mov rdi, 15
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #16, FPU Error ISR
kISRFPUError:
	KSAVECONTEXT

	mov rdi, 16
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #17, Alignment Check ISR
kISRAlignmentCheck:
	KSAVECONTEXT

	mov rdi, 17
	mov rsi, qword[rbp+8]
	call kCommonExceptionHandler

	KLOADCONTEXT
	add rsp, 8
	iretq

; #18, Machine Check ISR
kISRMachineCheck:
	KSAVECONTEXT

	mov rdi, 18
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #19, SIMD Floating Point Exception ISR
kISRSIMDError:
	KSAVECONTEXT

	mov rdi, 19
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

; #20~#31, Reserved ISR
kISRETCException:
	KSAVECONTEXT

	mov rdi, 20
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	interrupt handler
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; #32, Timer ISR
kISRTimer:
	KSAVECONTEXT

	mov rdi, 32
	call kCommonExceptionHandler

	KLOADCONTEXT
	iretq
