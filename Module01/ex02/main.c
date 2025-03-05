#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif
#include "avr/io.h"
#include "avr/interrupt.h"

// apport cyclique désigne, pour un phénomène périodique à deux états, 
// le rapport entre la durée de l'état actif et la période
// α = τ / T
// où
// α est le rapport cyclique (sans dimension) ;
// τ est le temps pendant lequel le signal est à l'état actif sur une période ;
// T est la période du signal.

int main(){
    DDRB |= (1 << PB1);

    // pour allumer la LED sans passer par PORT: 
    // PB1 alternate function: OC1A (table 14-3 page 91)
    // COM1A1 clear OC1A (table 16-1 page 140)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    // Pour activer le FastPWM mode - voir table 16-4 page 141
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    // Le prescaler divise la fréquence de l'horloge de l'ATmega328P pour ralentir le timer. Cela permet d'obtenir des périodes plus longues sans avoir à compter jusqu'à des valeurs très élevées.
    // Ici, nous utilisons un prescaler de 1024 pour ralentir le compteur du timer :
    // pour prescaler 1024: CS12 + CS10 (Table 16-5 page 143)
    TCCR1B |= (1 << CS12) | (1 << CS10);
    
    // Pour une fréquence de 1Hz avec un prescaler de 1024, 
    // nous avons besoin de définir ICR1 à 15624, car :
    // (FCPU / prescaler×frequence desiree) − 1 = (16×10^6 / 1024×1) −1 = 15624
    ICR1 = 15624; // pr 1 sec
    // OCR1A = 1562;
    OCR1A = (ICR1 / 10);

    while(1){}
    return 0;
}