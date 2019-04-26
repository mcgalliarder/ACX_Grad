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
void testThread(void);

int shiftCounter = 0;

int main(void)
{
   volatile int j = 0;
   x_init();
   x_new(0, thread0Blink, true);  // create thread, ID=1

   while(1){
	 j++;
	 x_yield();
   }
}

void testThread(void)
{
   volatile int i = 0;
   while(1){
      i++;
      x_yield();
   }
}

//------------------------
// A test thread
//------------------------
void thread0Blink()
{
   while(1){
	 //PORTF &= 0x80;
	 //PORTF |= (shiftCounter++) % 3;
	 //PORTF ^= 1 << 0;
     x_delay(1);
   }
}

void thread1Blink() {
	while(1) {
	  //PORTF &= 0x80;
	  //PORTF ^= 1 << 1;
	  x_delay(2);
	}
}

