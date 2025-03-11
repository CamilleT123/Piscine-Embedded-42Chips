#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"


ISR(INT0_vect){
    PORTB ^= (1 << PB0);
}

int main(){
    DDRB |= (1 << PB0);         // LED D1 is output
    DDRD &= ~(1 << PD2);        // SW1 is input
    // PORTD |= (1 << BUTTON_PIN); 
                                // Alternate function for PD2 is INT0 (table 14.9 page 97)
    EICRA |= (1 << ISC01);     //External Interrupt Control Register A
    EIMSK |= (1 << INT0); // External Interrupt Mask Register
    // EIFR = (1 << INTF0); // External Interrupt Flag Register
    sei();
    while(1){
        // PORTB = (1 << PB0);
    }
}