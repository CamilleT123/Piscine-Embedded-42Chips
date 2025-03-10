#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

/*
LED RGB blue = PD3 - alternate function: OC2B (Timer/Counter2 Output Compare Match B Output)
LED RGB red = PD5 - OC0B (Timer/Counter0 Output Compare Match B Output
LED RGB green = PD6 - OC0A (Timer/Counter0 Output Compare Match A Output)

Le PWM consiste à allumer et éteindre la LED rapidement (plusieurs milliers de fois par seconde), 
et en modulant la durée pendant laquelle la LED est allumée (le rapport cyclique), 
on peut contrôler l'intensité perçue de la LED.

Timer0 pour Red et Green - OCR0A pour intensite du rouge et OCR0B pour vert
Par exemple, si OCR0A = 255, le rouge sera à pleine intensité (100% allumé). 
Si OCR0A = 127, le rouge sera à 50% de son intensité maximale.
Timer2 pour Blue - OCR1A contrôle l'intensité du bleu (duty cycle).

Pour contrôler l'intensité des couleurs, nous devons ajuster les valeurs des 
registres OCR0A, OCR0B, et OCR1A 
en fonction des valeurs de couleur que nous souhaitons afficher. 
*/

void init_rgb(){ //initializes the timers
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);   // as output

    // TCCR0A = (1 << WGM00) | (1 << WGM01);  // sets FastPWM
    // TCCR0A |= (1 << COM0A1) | (1 << COM0B1); // Set OC0A and OC0B on Compare Match, set OC0B at BOTTOM (non-inverting mode).
    // TCCR0B |= (1 << CS01); // no prescaler

    TCCR2B = (1 << WGM21) | (1 << WGM00); //
    TCCR2B |= (1 << COM2B1) ; // quel PWM 10 ou 11
    TCCR2B |= (1 << CS20); // no prescaler (Table 16-5 page 143)

}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){ // allows you to set the color of the LED
//    OCR0A = r;
//    OCR0B = g;
   OCR1A = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } 
    else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } 
    else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(){
    init_rgb();

    while(1){
        for (uint8_t i = 0; i < 255; i++){
            wheel(i);
            _delay_ms(10);
        }
    }
    return 0;
}

