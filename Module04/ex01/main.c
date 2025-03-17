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


// timer1 gere le scientillement
// timer0 pour la variation de l'intensite
int dutycycle = 0;
int direction = 1;
int step = 1;

void init_timer0(){
    // mode normal
    TCCR0A = 0;
    TCCR0B = (1 << CS00) | (1 << CS02);       // prescaler 1024
    TIMSK0 = (1 << OCIE0A);             // activer l'interruption
    TIFR0 =  (1 << OCF0A);       
    // OCR0A = 62499; // OCR0A=((Periode×FCPU​) / Prescaler) ​−1 aveec period = 0.01 - interruptions toutes les 10ms
    // OCR0A = 31249;
    // OCR0A = 62;
}

void init_timer1(){
    TCCR1A |= (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
    TCCR1B |= (1 << WGM13)  ;

    TCCR1B |= (1 << CS10);

    ICR1 = 30;
    OCR1A = dutycycle;
}

ISR(TIMER0_COMPA_vect){

    if (dutycycle < 30 && direction == 1)
        dutycycle += step;
    if (dutycycle == 30)
        direction = 0;
    if (dutycycle > 0 && direction == 0)
        dutycycle -= step;
    if (dutycycle == 0)
        direction = 1;
    // if (dutycycle < 1.0 && direction == 1)
    //     dutycycle += 0.1;
    // else if (dutycycle >= 1.0){
    //     direction = 0;
    //     dutycycle -= 0.1;
    // }
    // else if (dutycycle <= 0.0){
    //     direction = 1;
    //     dutycycle += 0.1;
    // }
    // else if(dutycycle > 0.0 && direction == 0)
    //     dutycycle -= 0.1;

    OCR1A = dutycycle;
}

int main(){
    DDRB |= (1 << PB1); // LED = output - OC1A
    init_timer1();
    init_timer0();

    sei();
    
    while(1){}
    return 0;
}
