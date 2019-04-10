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


void testThread(void);

int main(void)
{
   DDRB = 0x80;
   PORTB |= 0x80;
   x_init();
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
     x_delay(1500);
	 //blink the LED
	 PORTB ^= 0x80;
   }
}

void thread1Blink(void) {
	while(1) {
	  x_delay(500);
	  //blink the LED
	  PORTB ^= 0x80;
	}
}

