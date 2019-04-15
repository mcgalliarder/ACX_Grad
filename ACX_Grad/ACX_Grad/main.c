/*
 * ACX_Grad
 * Created: 3/21/2019 2:26:59 PM
 * Author : Andrew Thorp thorpah
 * Author : Eli McGalliard mcgalliarder
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "acxserial.h"
#include "PSerial.h"
#include "acx.h"


void thread0Blink(void);
void thread1Blink(void);

int shiftCounter = 0;

int main(void)
{
   DDRB = 0x80;
   PORTB |= 0x80;
   DDRF |= (1<<DDF1)|(1<<DDF0);
   PORTF |= 0x03;
   x_init();
   setTimer();
   x_new(0, thread0Blink, true); 
   x_new(1, thread1Blink, true);  
   while(1){
	
   }
}

//------------------------
// A test thread
//------------------------
void thread0Blink(void)
{
   while(1){
	 //PORTF &= 0x80;
	 //PORTF |= (shiftCounter++) % 3;
	 //PORTF ^= 1 << 0;
     x_delay(500);
   }
}

void thread1Blink(void) {
	while(1) {
	  //PORTF &= 0x80;
	  //PORTF ^= 1 << 1;
	  x_delay(500);
	}
}

