#include "avr/io.h"
// section 14 page 84 xplains the ports
// section 14.4.2 PORTB – The Port B Data Register p100 shows the PORTB register

int main(){
    // D1 = PB0 - written on card
    DDRB = 1; // ajouter 1 au bit 0 - alternative : DDRB |= (1 << PB0);
    PORTB = 1; // ajouter 1 au bit 0 - PORTB |= (1 << PB0);

    while(1);
    return 0;
}