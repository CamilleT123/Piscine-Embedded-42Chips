#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

/*
LED RGB blue = PD3
LED RGB red = PD5
LED RGB green = PD6
*/

int main(){
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);   // as output

    while(1){
        PORTD = (PORTD & ~((1 << PD5) | (1 << PD6))) | (1 << PD3);
        _delay_ms(1000);
        PORTD = (PORTD & ~((1 << PD3) | (1 << PD6))) | (1 << PD5);
        _delay_ms(1000);
        PORTD = (PORTD & ~((1 << PD3) | (1 << PD5))) | (1 << PD6);
        _delay_ms(1000);
    }
    return 0;
}