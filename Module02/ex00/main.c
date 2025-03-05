#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "stdio.h"

#define BAUD 115200
#define UBRR ((F_CPU / (16 * BAUD)))  // Calcul du UBRR table 20-1 page 182 - sinon valeur page 196 table 20-4
// U2Xn (Universal Double Speed) dans le registre UCSRnA: 0 = normal, 1 = double

// USART Baud Rate Registers = UBRR - page 204 section 20.11.5
// UBRRnL - bits de 0 a 7?
// UBRRnH - bits de 8 a 15?
// UBRR[11:0] This is a 12-bit register which contains the USART baud rate. The UBRRnH contains the four most significant
// bits, and the UBRRnL contains the eight least significant bits of the USART baud rate. 
// UBRR[15:12] reserved

// Format des données (8 bits, 1 stop, pas de parité → 8N1) → UCSR0C
// USART Control and Status Register n C = UCSRnc page 202 section 20.11.4
// Bit 0 – UCPOLn: Clock Polarity - for synchronous mode only
// Bit 2:1 – UCSZn1:0: Character Size, combined with the UCSZn2 bit in UCSRnB sets the number of data bits in a frame
// Bit 3 – USBSn: Stop Bit Select, This bit selects the number of stop bits to be inserted by the Transmitter. The Receiver ignores this setting.
// Bits 5:4 – UPMn1:0: Parity Mode
// Bits 7:6 – UMSELn1:0 USART Mode Select 00 for asynchronous

// Activation de la transmission (TX) et réception (RX) → UCSR0B
// UCSRnB – USART Control and Status Register n B - page 201 section 20.11.3
// Bit	Nom	Description
// 7	RXCIE0	Interruption de réception : Active l'interruption quand des données sont reçues.
// 6	TXCIE0	Interruption de fin de transmission : Active l'interruption après qu'une transmission de données est terminée.
// 5	UDRIE0	Interruption de buffer vide : Active l'interruption quand le buffer de transmission (UDR) est vide, prêt à recevoir de nouvelles données.
// 4	RXEN0	Activation de la réception : Active le récepteur pour recevoir des données.
// 3	TXEN0	Activation de la transmission : Active le transmetteur pour envoyer des données.
// 2	UCSZ02	Taille des données (9 bits) : Lorsque ce bit est activé (1), l'UART fonctionne en mode 9 bits.
// 1	RXB8	Bit de données reçues 8ème (9 bits de données) : Utilisé pour les données 9 bits, ce bit contient le 9e bit lors de la réception.
// 0	TXB8	Bit de données transmises 8ème (9 bits de données) : Utilisé pour les données 9 bits, ce bit contient le 9e bit lors de la transmission.

void uart_init(){ //initializes the UART
    /*Set baud rate */
    // int ubbr = (F_CPU / (16 * BAUD)) - 1;
    UBRR0H = (unsigned char)(UBRR >> 8); // = 0
    UBRR0L = (unsigned char)UBRR; // = 8

    /*Enable transmitter*/
    UCSR0B = (1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    // UCSR0C = (1<<USBS0)|(3<<UCSZ00);

    // pour  8 bits, pas de parité, 1 bit de stop (8N1) :
    // UCSZ01:0 = charactere size si 1 1 et UCSZn2 in UCSRnB = 0 alors 8 bits
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Cette fonction va charger le caractère à transmettre dans le registre de données de l'UART
//  et s'assurer que la transmission se fait correctement.
void uart_tx(char c){ //writes a character to the PC serial port / code frin 20.6.1 page 186
    while (!(UCSR0A & (1 << UDRE0)));  // Attendre que le buffer soit prêt
    // Le bit UDRE0 dans le registre UCSR0A indique si le buffer de transmission (UDR0) 
    // est prêt à recevoir un nouveau caractère.
    // UCSR0A est un registre qui contient plusieurs bits d'état et de contrôle pour l'UART, 
    // et UDRE0 est un bit qui devient 1 lorsque le buffer de transmission est vide, 
    // c'est-à-dire prêt à recevoir de nouvelles données.
    // while attend que UDRE0 passe a 1
    UDR0 = c;  // Envoyer le caractère
    // UDR0 (USART Data Register 0) est un registre spécial pour transmettre et recevoir des données via le module USART
}

int main(){
    uart_init();
    while(1){
        uart_tx('Z');
        _delay_ms(1000);
    }
    return 0;
}

// The AVR ATmega328P microcontroller has 1 UART device that you must use in
// this exercise to communicate with a computer.

// • On the PC, the screen program is used to read the serial port from a terminal.

// • The MCU’s UART must be configured as 115200 8N1.
// • UBRRnL must be calculated based on BAUD and F_CPU.
// • The program should write ’Z’ to the serial port at 1Hz (do as you wish).