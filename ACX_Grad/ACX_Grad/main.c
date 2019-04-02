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

int main(void)
{
    DDRB |= 0x80;

    //PSerial_open(0, BAUD115200, SERIAL_8N1);

	x_init();
	
    while (1) {
		

		x_yield();

    }
}

