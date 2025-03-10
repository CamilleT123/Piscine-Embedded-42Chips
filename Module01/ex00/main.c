#include "avr/io.h"
#include "util/delay.h"

// SET un bit : register |= (1 << bit_position)
// CLEAR un bit : register &= ~(1 << bit_position)
// TOGGLE un bit : register ^= (1 << bit_position)
// TEST un bit : if (register & (1 << bit_position))


// The Port B Data Direction Register is set to 1 on bit PB1 (output mode) - section 14.4.3 page100
// The Port B Data Register is toggled  - section 14.4.2 page100
// between each toggle, delay for 1 cycle * 1600 * 1000 so for a seconde approximately as the frequence is 16*10^6
int main(){
    DDRB |= (1 << PB1);

    while(1){
        PORTB ^= (1 << PB1);
        for(unsigned int i = 0; i < 1600; i++){
            for(unsigned int j = 0; j < 1000; j++){
                asm volatile("nop");
                // asm : on inclut du code en langage d'assemblage. 
                // Le compilateur va generer ce code specifique au processeur, permet d'executer des instructions directement au niveau materiel.
                // volatile : indique au compilateur que l'instruction ne doit pas etre optimisee ou supprimee. 
                // "nop" : instruction d'assemblage qui signifie "No Operation", utilisee pour inserer une pause d'un cycle d'horloge                
            }
        }
    }
}