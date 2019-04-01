
/*
 * acx_asm.S
 *
 * Created: 
 *  Author: Andrew Thorp:  thorpah
 *  Author: Eli McGalliard: 
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
		;r24 containts thread ID input
		
		
		ret
