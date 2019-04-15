
/*
 * acx_asm.S
 *
 *  Created: 3/28/2019
 *  Author: Andrew Thorp thorpah
 *  Author: Eli McGalliard mcgalliarder 
 *
 *
 *       **************************************************
 *       **************************************************
 *       **     _   _   _                _______   __    **
 *       **    | | (_) | |         /\   / ____\ \ / /    **
 *       **    | |  _  | |__      /  \ | |     \ V /     **
 *       **    | | | | | '_ \    / /\ \| |      > <      **
 *       **    | | | | | |_) |  / ____ \ |____ / . \     **
 *       **    |_| |_| |_.__/  /_/    \_\_____/_/ \_\    **
 *       **                                              **
 *       **                                 Andrew Thorp **
 *       **                               Eli McGalliard **
 *       **************************************************
 *       **************************************************
 */

 #define __SFR_OFFSET 0
 #include <avr/io.h>
 #include "acx.h"



		.section .text
__SP_L__ =  0x3d
__SP_H__ =  0x3e
		.global x_yield
x_yield:
;------------------------------------------------------------------------
; Save thread context
;-------------------------------------------------------------------------

    ; Save registers for calling thread
	push r1 //Just in case
	push r2 // Save and restore callee-save registers
	push r3 // Push onto calling thread's stack
	push r4
	push r5
	push r6
	push r7
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29

	; save stack pointer in appropriate array location
    ;---------------------------
;	in r24, 

 	lds r30, x_thread_id        ; get stack ID
 	ldi r31, 0

 	lsl r30                     ; get stack control table offset
 	rol r31
 	lsl r30
 	rol r31
 	sbci r30, lo8(-(stackControlTable)) ; get stack pointer address from table
 	sbci r31, hi8(-(stackControlTable))

	in	r24, __SP_L__			; Put SP in r24-25
	in	r25, __SP_H__

	st	Z+,	r24					; *Z = SP 
	st	Z , r25


	; Check for stack overflows
    ;---------------------------
 	
 	; int * Z = mem
	ldi r30, lo8(mem)           ; Init Z pointer to &mem
 	ldi r31, hi8(mem)

 	; threadID * stacksize for canary
	; int offset = x_thread_id * 256
	ldi r24, 0
	lds r25,	x_thread_id

	add r31,	r25;
	ld	r24,	Z

	cpi	r24,	0xAA
	breq noSOF
SOF:
	; Execute if there is a stack overflow
	call x_stack_overflow
noSOF:
	; No stack overflow
 
	
;------------------------------------------------------------------------
; Schedule next thread -- must use caller save registers below here
;                         because we can reach this without saving regs
;                         if reached from k_new() function
;-------------------------------------------------------------------------
		.global	x_schedule
x_schedule:

	; Determine next thread to run
    ;---------------------------
	lds r18, x_thread_id
	
	lds r19, disable
	lds r20, suspend
	lds r21, delay

	or r21, r20
	or r21, r19

scan_begin:
	subi r18, -1			; get next thhread
	andi r18, 0x07			; mod by numthreads

	mov r24, r18
	call bit2mask8			; get mask of bit (thread_id + 1) % numthreads

	and r21, r24
	cp r21, r24

	breq scan_begin


	; Retrieve next stack pointer for next thread
    ;---------------------------
	sts x_thread_id, r18
	mov r30, r18        ; get stack ID
 	ldi r31, 0

 	lsl r30             ; get stack control table offset
 	rol r31
 	lsl r30
 	rol r31
 	sbci r30, lo8(-(stackControlTable)) ; get stack pointer address from table
 	sbci r31, hi8(-(stackControlTable))

	ld	r24, Z+					
	ld	r25, Z

    ; restore new thread's stack pointer
    ;---------------------------
    cli             ; begin stack pointer change		
	out __SP_L__, r24
	out __SP_H__, r25
    sei             ; end  stack pointer change

;---------------------------------------------------
; Restore context of next READY thread
;---------------------------------------------------
restore:
 	
    ; restore new thread's registers 
    ;---------------------------
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1

	ret


//----------------------------------------------------------------------------------------------
//  Function: bit2mask8
//
//  Description:   Returns a bit mask corresponding to thread ID parameter.
//
//  Input params:  uint8_t id  -- must by integer between 0 and 7
//  Returns:       mask - a bit mask having a single bit set corresponding to input ID
//						  (interpreted as bit number)
//-----------------------------------------------------------------------------------------------

		.global bit2mask8
bit2mask8:
		;r24 contains thread ID input
 		subi r24, -1
		ldi r25, 1
again:

		dec r24
		cpi r24, 1
		brlo done
		lsl r25
		jmp again

done:	mov r24, r25
		ldi r25, 0
		ret

