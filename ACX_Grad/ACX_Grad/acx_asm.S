
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


//lookup: .db 1, 2, 4, 8, 16, 32, 64, 128
		
		.global x_yield
x_yield:
;------------------------------------------------------------------------
; Save thread context
;-------------------------------------------------------------------------

; your code here...

	push r8 // Save and restore callee-save registers
	push r9 // Push onto calling thread's stack



	//When exiting pop them from stack in reverse order
	pop r9
	pop r8
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
		//cp 0, r24
		//brlo none
		//cp r24, 8
		//brge none
		ldi r25, 1
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


