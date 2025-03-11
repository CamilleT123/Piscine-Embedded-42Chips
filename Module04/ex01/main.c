#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

// Timer1 controle le duty cycle de la LED - frequency high enough not to blink
// Timer0 triggers a periodic interrup qui fait varier le duty cycle
// duty cycle de 0 a 100% puis 100 a 0 en 1 sec
// Timer0 : en mode CTC, donc genere une interruption a chaque fois qu'il atteint la valeur de OCR0A
// si on veut generer une interruption toutes les 1ms
// calcul de la periode d'interruption : Periode = ((OCR0A+1)*prescaler) / FCPU
// donc 0.001 * 16 000 000 = OCR0A+1 * 256 == OCR0A = 62
float dutycycle = 0;
int direction = 1;

void init_timer0(){
    TCCR0A = (1 << WGM01); // mode CTC
    TCCR0B = (1 << CS00) | (1 << CS02); // prescaler 1024
    TIMSK0 = (1 << OCIE0A); // activer l'interruption
    OCR0A = 155; // OCR0A=((Periode×FCPU​) / Prescaler) ​−1 aveec period = 0.01 - interruptions toutes les 10ms
}

void init_timer1(){
    TCCR1A |= (1 << COM1A1) | (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) ;

    // pour prescaler 256: CS12 (Table 16-5 page 143)
    TCCR1B |= (1 << CS12);
    dutycycle = 0.1;

    // ICR1= (FCPU / prescaler × frequence desiree) − 1 = (16×10^6 / 256) −1 = 62499 /
    uint32_t temp = (uint32_t)(F_CPU / (256));
    ICR1 = (uint16_t)(temp / 1000) - 1; 
    OCR1A = ICR1 * dutycycle;
}

ISR(TIMER0_COMPA_vect){
    if (dutycycle < 1.0 && direction == 1)
        dutycycle += 0.1;
    else if (dutycycle >= 1.0){
        direction = 0;
        dutycycle -= 0.1;
    }
    else if (dutycycle <= 0.0){
        direction = 1;
        dutycycle += 0.1;
    }
    else if(dutycycle > 0.0 && direction == 0)
        dutycycle -= 0.1;

    OCR1A = ICR1 * dutycycle;
}

int main(){
    DDRB |= (1 << PB1); // LED = output - OC1A
    init_timer0();
    init_timer1();

    while(1){}
    return 0;
}
