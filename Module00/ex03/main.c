#include "avr/io.h"
#include "util/delay.h"

// & ET bit a bit: compare bits du registre et du masque
// ligne extrait UNIQUEMENT le bit PD2

// ^= XOR inverse l'état d'un bit spécifique : si le bit est 0, il devient 1, et si le bit est 1, il devient 0.

int main(){

    DDRB |= (1 << PB0);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    uint8_t last_state = 1;
    uint8_t current_state;

    while(1){
        current_state = PIND & (1 << PD2);
        if (!current_state && current_state != last_state){
            // si la broche est a 0 / si PD2 est bas, le bouton est appuye
            PORTB ^= (1 << PB0);
            _delay_ms(500);
        }
        last_state = current_state;
    }

    return 0;
}