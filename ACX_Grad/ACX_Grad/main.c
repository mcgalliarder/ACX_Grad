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

    PSerial_open(0, BAUD115200, SERIAL_8N1);
	
	int i;
	
	for (int j = 0; j < SP; j++) {
		i = (int) SP;
	}
	
	/*
    x_init();

    _delay_ms(5000);
    PSprintf(0, "T0 Stack pointer: %X\n\r", mem + T0_STACK_BASE_OFFS);
    _delay_ms(1000);
    */
    while (1) {
		/*
        PSprintf(0, "Stack: %X\n\r", SP);
        asm("push 0x1");
        PORTB ^= 0x80;
        _delay_ms(100);
		*/

		x_yield();

    }
}

