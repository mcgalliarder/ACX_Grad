/*
 * acx.c
 *
 * Created: 3/26/2019 12:34:55 PM
 *  Author: Andrew Thorp
 * Author: Eli McGalliard mcgalliarder
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
 *
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include "acx.h"


byte disable;
byte suspend;
byte delay;
//unsigned int delayCounters[MAX_DELAY];
uint16_t x_thread_delay[MAXTHREADS]; //Each thread has a max_delay counter
byte x_thread_id;
byte x_thread_mask;
byte mem[STACK_MEM_SIZE];

stackControl stackControlTable [MAXTHREADS] = {{T0_STACK_BASE_OFFS + (int) mem, T0_STACK_BASE_OFFS+ (int) mem},
											   {T1_STACK_BASE_OFFS + (int) mem, T1_STACK_BASE_OFFS+ (int) mem},
											   {T2_STACK_BASE_OFFS + (int) mem, T2_STACK_BASE_OFFS+ (int) mem},
											   {T3_STACK_BASE_OFFS + (int) mem, T3_STACK_BASE_OFFS+ (int) mem},
											   {T4_STACK_BASE_OFFS + (int) mem, T4_STACK_BASE_OFFS+ (int) mem},
											   {T5_STACK_BASE_OFFS + (int) mem, T5_STACK_BASE_OFFS+ (int) mem},
											   {T6_STACK_BASE_OFFS + (int) mem, T6_STACK_BASE_OFFS+ (int) mem},
											   {T7_STACK_BASE_OFFS + (int) mem, T7_STACK_BASE_OFFS+ (int) mem}};

//---------------------------------------------------
// Initialize all kernel state variables
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


void x_init(void)
{
	// Save the stack pointer as a byte pointer
	byte * stackP = (byte *) SP;
	
	
	asm("cli");

    // initialize kernal data structures
    kernalInit();
    // place canary values
    placeCanaries();

    // Save the stack pointer as a byte pointer

    // create a PTUnion and save the return address
    volatile PTUnion ret;
    ret.addr[0] = * (stackP + 10);
    ret.addr[1] = * (stackP + 9);
    ret.addr[2] = * (stackP + 8);

    // change the stack pointer to the bottom of T0
    SP = (int) (mem + T0_STACK_BASE_OFFS);

    // push the old return address onto the new stack
	int stackpointer = SP - (int) mem;
	mem[stackpointer--] = ret.addr[0];
	mem[stackpointer--] = ret.addr[1];
	mem[stackpointer--] = ret.addr[2];
	SP = SP - 10;
	


	
	asm("sei");

	// return to caller.
}

void x_delay(unsigned int time) {
	if (time > MAX_DELAY) time = MAX_DELAY;
	cli();

	// Your initialization code here
	x_thread_delay[x_thread_id] = time;
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


/*
 * x_new initializes a new thread in the system
 * 
 * ID - the new thread ID (0 - 7). If a thread with the same ID already exists
 *		the new thread will replace the current thread of that ID
 * newThread -  a function pointer that takes no parameters and returns nothing. 
 * 				We may later change this to accept parameters.
 * isEnabled - the initial status of the thread--1 means enabled, 0 means disabled.
 */
void x_new(uint8_t ID, PTHREAD thread, bool enable) {
	cli();
	//volatile PTUnion ret = {*thread};
	volatile PTUnion ret;
	ret.addr[2] = 0;
	ret.addr[1] = 0;
	ret.addr[0] = 0;
	ret.pthread = thread;
	
	int stackpointer = stackControlTable[ID].spBase - (int) mem;
	
	mem[stackpointer--] = ret.addr[0];
	mem[stackpointer--] = ret.addr[1];
	mem[stackpointer--] = ret.addr[2];
	
	stackControlTable[ID].sp = stackControlTable[ID].spBase - 22; //18
	
	char disablebit = enable << ID;
	disable = (enable) ? disable & ~disablebit : disable | disablebit ;

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

// called from X_yield if the canary is malformed
void x_stack_overflow(void) {
	DDRB |= 0x80;
	while (1) {
		for (int i = 0; i < 3; i++) {
			_delay_ms(100);
			PORTB |= 0x80;
			_delay_ms(100);
			PORTB ^= 0x80;
		}
		for (int i = 0; i < 3; i++) {
			_delay_ms(300);
			PORTB |= 0x80;
			_delay_ms(300);
			PORTB ^= 0x80;
		}
		for (int i = 0; i < 3; i++) {
			_delay_ms(100);
			PORTB |= 0x80;
			_delay_ms(100);
			PORTB ^= 0x80;
		}
		_delay_ms(300);
	}
}

void setTimer(int threadsID) {
	PRR0 = 0x00;
	TCNT1 = 0;

	// period 333.33 ms, on-time = 75 ms
	int TOP1 = 16144; // OFFTIME
	int TOP2 = 4688;  // ONTIME

	cli();
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	//TCCR0A = 0x02; //Sets CTC mode of operation?

	// configure match register
	ICR1 = TOP1;
	OCR1A = TOP2;    // 75 msec
	TCCR1B = 0x04;   // clk/256 from prescaler

	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);

	// Set CS10 and CS12 bits for 1024 prescaler:
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);

	// enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);
	sei();          // enable global interrupts

	// setup initial output state
	DDRB = 0x80;
	PORTB |= 0x80;
}

ISR(TIMER1_COMPA_vect){
	//decrement x_thread_id's timer	
}
