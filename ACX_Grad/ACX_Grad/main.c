/*
 * ACX_Grad
 * Created: 3/21/2019 2:26:59 PM
 * Author : Andrew Thorp thorpah
 * Author : Eli McGalliard mcgalliarder
 */ 

#include <avr/io.h>
#include <stdbool.h>
//#include <acx.h>
#include <util/delay.h>
#include "acxserial.h"
#include "PSerial.h"
#include "acx.h"


void testThread(void);

int main(void)
{
	//int j;
	//for(int i = 0; i < 8; i++) j = bit2mask8(i);
   volatile int j = 0;
   x_init();
   x_new(1, testThread, true);  // create thread, ID=1
// x_new(0, testThread, true);  // replace current thread
   while(1){
      j++;
      x_yield();
   }
}

//------------------------
// A test thread
//------------------------
void testThread(void)
{
   volatile int i = 0;
   while(1){
      i++;
      x_yield();
   }
}

