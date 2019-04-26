/*
 * acx.c
 *
 * Created: 3/26/2019 12:34:55 PM
 * Author: Andrew Thorp thorpah
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
byte mem[STACK_MEM_SIZE];
byte x_thread_id;
byte x_thread_mask;
volatile uint16_t x_thread_delay[MAXTHREADS]; //Each thread has a max_delay counter
unsigned long x_gTime = 0;

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
        x_thread_delay[i] = 0; 

    x_thread_id = 0; // current thread
    x_thread_mask = 0x01;
	setTimer();
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

    // create a PTUnion and save the return address
    volatile PTUnion ret;
    ret.addr[0] = * (stackP + 8);
    ret.addr[1] = * (stackP + 9);
    ret.addr[2] = * (stackP + 10);

    // change the stack pointer to the bottom of T0
    SP = (int) (mem + T0_STACK_BASE_OFFS);

    // push the old return address onto the new stack
	int stackpointer = SP - (int) mem;
	mem[stackpointer--] = ret.addr[2];
	mem[stackpointer--] = ret.addr[1];
	mem[stackpointer--] = ret.addr[0];
	SP = SP - 10;
	
	asm("sei");
	// return to caller.
}

void x_delay(unsigned int time) {
	if (time > MAX_DELAY) time = MAX_DELAY;
	if (time < 0) time = 0; // shouldn't ever happen
	cli();
    
	// copy delay value into calling thread's counter
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		x_thread_delay[x_thread_id] = time;	
	}
	
	// set x_delay_status bit corresponding to thread's id 
	delay |= bit2mask8(x_thread_id); 
	
	// initiate thread rescheduling
	sei();
	x_yield(); 
	
	// return to caller.
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
	//x_thread_id = ID;
	x_thread_mask |= bit2mask8(ID);
	volatile PTUnion ret;

	ret.addr[2] = 0;
	ret.addr[1] = 0;
	ret.addr[0] = 0;

	ret.pthread = thread;
	
	int stackpointer = stackControlTable[ID].spBase - (int) mem;
	
	// since the int is stored in Little Endian,
	// you have to reverse it here so the return address
	// is big endian in memory.
	mem[stackpointer--] = ret.addr[0];
	mem[stackpointer--] = ret.addr[1];
	mem[stackpointer--] = ret.addr[2];
	
	stackControlTable[ID].sp = stackControlTable[ID].spBase - 22; //18
	
	char disablebit = enable << ID;
	disable = (enable) ? disable & ~disablebit : disable | disablebit ;

	sei();

	// return to caller.
}

unsigned long x_gtime() {
	
	return x_gTime;

}

void x_suspend(uint8_t ID) {
	cli();

	// Your initialization code here
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		suspend |= bit2mask8(ID);
	}
	sei();

	// return to caller.
}

void x_resume(uint8_t ID) {
	cli();

	// Your initialization code here
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		suspend &= ~bit2mask8(ID);
	}
	sei();

	// return to caller.
}

void x_disable(uint8_t ID) {
	cli();

	// Your initialization code here
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		disable |= bit2mask8(ID);
	}
	sei();

	// return to caller.
}

void x_enable(uint8_t ID) {
	cli();

	// Your initialization code here
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		disable &= ~bit2mask8(ID);
	}
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

void setTimer() {
	PRR0 = 0x00;
	TCNT1 = 0;

	// period 333.33 ms @ 16144, on-time = 75 ms @ 4688
	int TOP1 = 63; // OFFTIME - 1 ms
	int TOP2 = 63;//63;// ONTIME - 1 ms

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
}

ISR(TIMER1_COMPA_vect){
	
	x_gTime++;
	
	//check x_delay_thread for every thread
	for(int i = 0; i < MAXTHREADS; i++) {
		
		// check if thread is currently delayed
		int delayStatus = bit2mask8(i) & delay; 
		
		//if the delay status is not zero and the count isn't zero
		if (x_thread_delay[i] && delayStatus) { 
			
			// decrement threads count
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
				x_thread_delay[i]--;
			}
			
			//if counter is now zero then clear delay bit
			if (!x_thread_delay[i])
				delay &= ~(bit2mask8(i));
		}
	}
}
