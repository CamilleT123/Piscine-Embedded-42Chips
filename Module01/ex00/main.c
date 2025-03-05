#include "avr/io.h"
#include "util/delay.h"

// SET un bit : register |= (1 << bit_position)
// CLEAR un bit : register &= ~(1 << bit_position)
// TOGGLE un bit : register ^= (1 << bit_position)
// TEST un bit : if (register & (1 << bit_position))

int main(){
    DDRB |= (1 << PB1);

    while(1){
        PORTB ^= (1 << PB1);
        for(unsigned int i = 0; i < 1600; i++){
            for(unsigned int j = 0; j < 1000; j++){
                asm volatile("nop");
                // asm : Cela signifie qu'on inclut du code en langage d'assemblage. Le compilateur va générer ce code spécifique au processeur ciblé. Cela permet d'exécuter des instructions directement au niveau matériel.
                // volatile : Cela indique au compilateur que l'instruction ne doit pas être optimisée ou supprimée. 
                // "nop" : C'est une instruction d'assemblage qui signifie "No Operation" (aucune opération). Elle est souvent utilisée pour insérer une pause d'un cycle d'horloge dans le code
                
            }
        }
    }
    return 0;
}