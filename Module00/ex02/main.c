#include "avr/io.h"
#include "util/delay.h"

// section 14 page 84 xplains the ports
// section 14.4.2 PORTB – The Port B Data Register p100 shows the PORTB register
// D1 = PB0 - written on card
// SW1 = PD2 - written on card and subject

// pull-up (resistance de tirage) : 
// Les microcontrôleurs AVR, comme le ATMega328p, ont des pull-ups internes intégrées, qui peuvent être activées via le registre PORTx. 
// Sans résistance de tirage : La broche est laissée flottante  (c'est-à-dire non connectée à une source de signal définie),
// ce qui peut conduire à des comportements imprévisibles.
// Avec résistance de tirage : La broche est connectée à une source de tension (Vcc),
// la resistance stabilise l'état en forçant le signal à un niveau logique connu (haute lorsque le bouton est relâché).
// Si le bouton est relâché, la broche doit être tirée vers Vcc pour qu'elle soit lue comme étant haute (logique 1).
// Si le bouton est appuyé, il connecte directement la broche à la masse (GND), ce qui la tire vers 0 (logique 0).

// |=  OR 
// a |= b; est équivalent à a = a | b;
// c'est-à-dire qu'on met à 1 dans a tous les bits qui sont à 1 dans b.
// permet de garder a 1 les bits qui etaient deja a 1 - ne les change pas

// ~  NOT -> met a zero le bit de PD2
// &=  ET, bit a bit entre valeur actuelle du registre et masque
// donc les 1 restent a 1 si le masque est de 1
// et les 0 du masque passeront le registre a 0
// garde les bits existants sauf pour PD2 donc ne modifie pas les autres bits

// & ET bit a bit: compare bits du registre et du masque
// ligne extrait UNIQUEMENT le bit PD2 donc

int main(){
    DDRB |= (1 << PB0);
    // DDRB = 1; PB0 as output - section 14.4.3 page100
    // PB0 est le bit 0 de DDRB,
    
    DDRD &= ~(1 << PD2);
    // DDRD = 0; PD2 as input - section 14.4.9 page100

    PORTD |= (1 << PD2);
    // active la résistance de tirage pour le bouton.

    while(1){
        if (!(PIND & (1 << PD2))){
        // PINx is read only: verifie l'etat du PD2 - section 14.1 page 84
        // si PIND & (1 << PD2) = 00000100 donc PD2 est haut
        // si !(PIND & (1 << PD2)) = si PD2 est bas = si bouton appuye
            PORTB |= (1 << PB0); // allume la LED
        }
        else{
            PORTB &= ~(1 << PB0); // sinon LED eteinte
        }
    }
    return 0;
}