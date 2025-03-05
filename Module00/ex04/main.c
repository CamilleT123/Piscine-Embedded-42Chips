#include "avr/io.h"
#include "util/delay.h"

int main(){
    // D1 = PB0, D2 = PB1, D3 = PB2, D4 = PB4 - tout en output
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);

    // SW1 = PD2, SW2 = PD4 - tout en input
    DDRD &= ~((1 << PD2) | (1 << PD4));
    PORTD |= (1 << PD2) | (1 << PD4);

    uint8_t last_state1 = 1;
    uint8_t current_state1 = 0;
    uint8_t last_state2 = 1;
    uint8_t current_state2 = 0;

    uint8_t value = 0;
    while(1){
        current_state1 = PIND & (1 << PD2); // si SW1 appuye
        if (!current_state1 && current_state1 != last_state1){
            if (value < 15)
                value++;
            _delay_ms(200);
        }
        last_state1 = current_state1;
        current_state2 = PIND & (1 << PD4); // si SW2 appuye
        if (!current_state2 && current_state2 != last_state2){
            if (value > 0)
                value--;
            _delay_ms(200);
        }
        last_state2 = current_state2;

        PORTB = (PORTB & (1 << PB3)) |(value & 0x0F) | ((value & (1 << 3)) ? (1 << PB4) : 0);
        // 0x0F = masque de bits en hexa : 00001111
        // value & OxOF ne conserve que les 4 premiers bits de value, les suivants sont mis a 0
        // (value & (1 << 3)) : masque qui cible le bit 3 (4e bit) de value
        // si ce bit est 1 -> test est vrai donc defini PB4 a 1 avec (1 << PB4)
        // si ce bit est 0 -> test est faux donc 0 - pas de modif de PB4
        // pour proteger PB3 qui ne doit pas etre modifie : (1 << PB3) masque ciblant le bit 3
        // ET -> seul PB3 est conserve as is et tous les autres sont mis a 0
        // PORTB & ~(1 << PB3) masque le bit de PB3 pour ne pas le modifier
        _delay_ms(50);
    }
    return 0;
}