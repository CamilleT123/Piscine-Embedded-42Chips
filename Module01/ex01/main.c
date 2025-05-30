#include "avr/io.h"
#include "avr/interrupt.h"

// Le mode CTC permet au timer de compter jusqu'à une valeur spécifiée dans
// le registre OCR1A (Output Compare Register 1A). Lorsqu'il atteint cette valeur, il génère un événement (ou une interruption), et le compteur est réinitialisé à 0. 
// TCCR1B (Timer/Counter Control Register B) : Ce registre est utilisé pour
//  configurer le mode de fonctionnement du timer et le prescaler.

int main(){
    // The Port B Data Direction Register is set to 1 on bit PB1 (output mode) - section 14.4.3 page100
    DDRB |= (1 << PB1);

    // Pour activer le CTC mode, on configure WGM12 dans TCCR1B - voir table 16-4 page 141
    TCCR1B |= (1 << WGM12);
    
    // Pour une fréquence de 1Hz avec un prescaler de 1024, 
    // nous avons besoin de définir OCR1A à 15624, car :
    // OCR1A= (FCPU / prescaler×frequence desiree) − 1 = (16×10^6 / 1024×1) −1 = 15624 /
    // OCR1A = 15624;

    // Pour une fréquence de 1Hz avec un prescaler de 256, 
    // nous avons besoin de définir OCR1A à 62499, car :
    // OCR1A= (FCPU / prescaler×frequence desiree) − 1 = (16×10^6 / 256) −1 = 62499 /
    OCR1A = 62499;

    // Le prescaler divise la fréquence de l'horloge de l'ATmega328P pour ralentir le timer. Cela permet d'obtenir des périodes plus longues sans avoir à compter jusqu'à des valeurs très élevées.
    // Ici, nous utilisons un prescaler de 1024 pour ralentir le compteur du timer :
    // pour prescaler 1024: CS12 + CS10 (Table 16-5 page 143)
    // TCCR1B |= (1 << CS12) | (1 << CS10);
     // pour prescaler 256: CS12 (Table 16-5 page 143)
    TCCR1B |= (1 << CS12);

    // pour allumer la LED sans utiliser PORT: Toggle OC1A on compare match - table 16-1 page 140
    // table 14-3 page 91 pour voir que OC1A est alternate function de PB1
    TCCR1A |= (1 << COM1A0);

    while(1){}
    return 0;
}