
/*
 * acx_asm.S
 *
 *  Created: 3/28/2019
 *  Author: Andrew Thorp thorpah
 *  Author: Eli McGalliard mcgalliarder 
 */

 #define __SFR_OFFSET 0
 #include <avr/io.h>
 #include "acx.h"



		.section .text
__SP_L__ =  0x3e
__SP_H__ =  0x3d
		.global x_yield
x_yield:
;------------------------------------------------------------------------
; Save thread context
;-------------------------------------------------------------------------

    ; Save registers for calling thread
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

	; save stack pointer in appropriate array location
    ;---------------------------
;	in r24, 

 	lds r30, x_thread_id        ; get stack ID
 	ldi r31, 0

 	lsl r30                     ; get stack control table offset
 	rol r31
 	lsl r30
 	rol r31
 	subi r30, lo8(-(stackControlTable)) ; get stack pointer address from table
 	subi r31, hi8(-(stackControlTable))

	in r24, __SP_L__
	in r25, __SP_H__

	st Z+ , r24
	st Z , r25


	; Check for stack overflows
    ;---------------------------


 	
 	ldi r30, lo8(mem)           ; Init Z pointer to &mem
 	ldi r31, hi8(mem)
 	
 	
	; Determine next thread to run
 	; threadID * stacksize for canary
    ;---------------------------

	; Retrieve next stack pointer for next thread
    ;---------------------------

    ; restore new thread's stack pointer
    ;---------------------------
    cli             ; begin stack pointer change
        ; do stack pointer change here
    sei             ; end  stack pointer change


    ; restore new thread's registers 
    ;---------------------------
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

	ret
	
;------------------------------------------------------------------------
; Schedule next thread -- must use caller save registers below here
;                         because we can reach this without saving regs
;                         if reached from k_new() function
;-------------------------------------------------------------------------
		.global	x_schedule
x_schedule:

; your code here



;---------------------------------------------------
; Restore context of next READY thread
;---------------------------------------------------
restore:

; your code here


	// Return to next thread
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
		cpi r24, 1
		brlo none
		/*
		cp 8, r24
		brge none

		ldi r25, 1
		*/
 		adc r24, 1
again:

		dec r24
		cpi r24, 1
		brlo done
		lsl r25
		jmp again

done:	mov r24, r25
		ldi r25, 0
		jmp finished

none:
		ldi r25, 0
		ldi r24, 0	
finished:
		ret

