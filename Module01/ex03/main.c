#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

int main(){
    DDRB |= (1 << PB1);

    // pour allumer la LED sans passer par PORT: 
    // PB1 alternate function: OC1A (table 14-3 page 91)
    // COM1A1 clear OC1A (table 16-1 page 140)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    // Pour activer le FastPWM mode - voir table 16-4 page 141
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    
    // TCCR1B |= (1 << CS12) | (1 << CS10);
    TCCR1B |= (1 << CS12);

    
    // Pour une fréquence de 1Hz avec un prescaler de 1024, 
    // nous avons besoin de définir ICR1 à 15624, car :
    // (FCPU / prescaler×frequence desiree) − 1 = (16×10^6 / 1024×1) −1 = 15624
    // ICR1 = (15624); // pr 1 sec
    ICR1 = 62499;

    
    // SW1 = PD2
    // SW2 = PD4
    DDRD &= ~((1 << PD2) | (1 << PD4));
    
    float dutycycle = 0.1;
    uint8_t last_state1 = 1;
    uint8_t current_state1 = 0;
    uint8_t last_state2 = 1;
    uint8_t current_state2 = 0;

    OCR1A = ICR1 * dutycycle; 
    while(1){
        current_state1 = PIND & (1 << PD2); // si SW1 appuye
        if (!current_state1 && current_state1 != last_state1){
            if (dutycycle < 1.0){
                dutycycle += 0.1;
                OCR1A = ICR1 * dutycycle;
            }
            _delay_ms(200);
        }
        last_state1 = current_state1;
        current_state2 = PIND & (1 << PD4); // si SW2 appuye
        if (!current_state2 && current_state2 != last_state2){
            if (dutycycle > 0.0){
                dutycycle -= 0.1;
                OCR1A = ICR1 * dutycycle;
            }
            _delay_ms(200);
        }
        last_state2 = current_state2;
    }
    return 0;
}