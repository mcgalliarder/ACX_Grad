
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
		cp r24, 0
		brlo none
		cp 8, r24
		brge none
		ldi r25, 1
again:

		dec r24
		cp 0, r24
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
/*
		cp r24, 0
		brne one
		ldi r24, 1
		jmp done
one:
		cp r24, 1
		brne two
		ldi r24, 2
		jmp done
two:
		cp r24, 2
		brne three
		ldi r24, 4
		jmp done
three:
		cp r24, 3
		brne four
		ldi r24, 8
		jmp done
four:
		cp r24, 4
		brne five
		ldi r24, 16
		jmp done
five:
		cp r24, 5
		brne six
		ldi r24, 32
		jmp done
six:
		cp r24, 6
		brne seven
		ldi r24, 64
		jmp done
seven:
		cp r24, 7
		brne none
		ldi r24, 128
		jmp done
none:
		ldi r24, 0
*/
