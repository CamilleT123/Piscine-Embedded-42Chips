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
        PORTD = (PORTD & ~((1 << PD5) | (1 << PD6))) | (1 << PD3); // blue
        _delay_ms(1000);
        PORTD = (PORTD & ~((1 << PD3) | (1 << PD6))) | (1 << PD5); // red
        _delay_ms(1000);
        PORTD = (PORTD & ~((1 << PD3) | (1 << PD5))) | (1 << PD6); // greem
        _delay_ms(1000);
        PORTD = (PORTD & ~(1 << PD3))| (1 << PD5) | (1 << PD6); // yellow
        _delay_ms(1000);
        PORTD = (PORTD & ~(1 << PD5)) | (1 << PD3) | (1 << PD6); // cyan
        _delay_ms(1000);
        PORTD = (PORTD & ~(1 << PD6)) | (1 << PD5) | (1 << PD3); // magenta
        _delay_ms(1000);
        PORTD = (1 << PD6) | (1 << PD5) | (1 << PD3); // white
        _delay_ms(1000);
    }
    return 0;
}
//   // Pour activer le CTC mode, on configure WGM12 dans TCCR1B - voir table 16-4 page 141
//   TCCR1B |= (1 << WGM12);
  
//   OCR1A = 62499;
//   TCCR1B |= (1 << CS12);

//   // pour allumer la LED sans utiliser PORT: Toggle OC1A on compare match - table 16-1 page 140
//   // table 14-3 page 91 pour voir que OC1A est alternate function de PB1
//   TCCR1A |= (1 << COM1A0);