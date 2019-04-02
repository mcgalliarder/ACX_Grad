/*
 * acx.c
 *
 * Created: 3/26/2019 12:34:55 PM
 *  Author: Andrew Thorp
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"

//---------------------------------------------------
// Initialize all kernal state variables
//---------------------------------------------------
void kernalInit(void) {
    disable = 0xFE;
    suspend = 0x00;
    delay = 0x00;

    for (int i = 0; i < MAXTHREADS; i++)
        // no delays
        delayCounters[i] = 0; 

    x_thread_id = 0; // current thread
    x_thread_mask = 0x01;

    stackControlTable = {{T0_STACK_BASE_OFFS + mem, T0_STACK_BASE_OFFS + mem},
                         {T1_STACK_BASE_OFFS + mem, T1_STACK_BASE_OFFS + mem},
                         {T2_STACK_BASE_OFFS + mem, T2_STACK_BASE_OFFS + mem},
                         {T3_STACK_BASE_OFFS + mem, T3_STACK_BASE_OFFS + mem},
                         {T4_STACK_BASE_OFFS + mem, T4_STACK_BASE_OFFS + mem},
                         {T5_STACK_BASE_OFFS + mem, T5_STACK_BASE_OFFS + mem},
                         {T6_STACK_BASE_OFFS + mem, T6_STACK_BASE_OFFS + mem},
                         {T7_STACK_BASE_OFFS + mem, T7_STACK_BASE_OFFS + mem}};
}

//---------------------------------------------------
// Stack Control
//---------------------------------------------------
void placeCanaries(void) {
    mem[T0_CANARY_OFFS] = CANARY;
    mem[T1_CANARY_OFFS] = CANARY;
    mem[T2_CANARY_OFFS] = CANARY;
    mem[T3_CANARY_OFFS] = CANARY;
    mem[T4_CANARY_OFFS] = CANARY;
    mem[T5_CANARY_OFFS] = CANARY;
    mem[T6_CANARY_OFFS] = CANARY;
    mem[T7_CANARY_OFFS] = CANARY;
}

//---------------------------------------------------
// Stack Memory
//---------------------------------------------------

//---------------------------------------------------
// Thread Delay Counters
//---------------------------------------------------


//---------------------------------------------------
// Exec State Variables
//---------------------------------------------------



//---------------------------------------------------
// Local Functions
//---------------------------------------------------


//---------------------------------------------------
// ACX Functions
//---------------------------------------------------
//
// defined in acx_asm.S
// void x_yield(void)
// void x_schedule(void)

void x_init(void)
{
	cli();

    // initialize kernal data structures
    kernalInit();
    // place canary values
    placeCanaries();

    // Save the stack pointer as a byte pointer
    byte * stackP = (byte *) SP;

    // create a PTUnion and save the return address
    PTUnion ret;
    ret.addr[0] = * (stackP - 1);
    ret.addr[1] = * (stackP - 2);
    ret.addr[2] = * (stackP - 3);

    // change the stack pointer to the bottom of T0
    SP = (int) (mem + T0_STACK_BASE_OFFS);

    // push the old return address onto the new stack
    asm("push %0" : "=r" (ret.addr[2]));
    asm("push %0" : "=r" (ret.addr[1]));
    asm("push %0" : "=r" (ret.addr[0]));

	sei();

	// return to caller.
}

void x_delay(unsigned int time) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

unsigned long x_gtime() {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
	return 1L;
}



void x_new(uint8_t ID, PTHREAD thread, bool enable) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

void x_suspend(uint8_t ID) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

void x_resume(uint8_t ID) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

void x_disable(uint8_t ID) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

void x_enable(uint8_t ID) {
	cli();

	// Your initialization code here

	sei();

	// return to caller.
}

