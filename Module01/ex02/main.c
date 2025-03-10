#include "avr/io.h"
#include "avr/interrupt.h"

// apport cyclique désigne, pour un phénomène périodique à deux états, 
// le rapport entre la durée de l'état actif et la période
// α = τ / T où α est le rapport cyclique (sans dimension) ;
// τ est le temps pendant lequel le signal est à l'état actif sur une période ;
// T est la période du signal.

// differents modes de timers :
// Normal Mode – Timer counts up to TOP, then overflows
// CTC Mode (Clear Timer on Compare Match) – Resets timer at OCRnA
// Phase Correct PWM, Timer counts up & down (smoother PWM)
// Fast PWM, High-speed PWM (resets at TOP)

int main(){
    DDRB |= (1 << PB1);

    // pour allumer la LED sans passer par PORT: 
    // PB1 alternate function: OC1A (table 14-3 page 91)
    // COM1A1 clear OC1A (table 16-1 page 140)
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    // Pour activer le FastPWM mode - voir table 16-4 page 141
    TCCR1B |= (1 << WGM13) | (1 << WGM12);

    // pour prescaler 256: CS12 (Table 16-5 page 143)
    TCCR1B |= (1 << CS12);
    
    // OCR1A= (FCPU / prescaler×frequence desiree) − 1 = (16×10^6 / 256) −1 = 62499 /
    ICR1 = 62499;
    OCR1A = (ICR1 / 10);

    while(1){}
    return 0;
}