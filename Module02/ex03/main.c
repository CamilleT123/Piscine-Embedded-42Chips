#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

/*
RX = PD0 et TX = PD1 - written on card

Explication on section 20.5 page 184:
initialization consists of setting the baud rate, 
setting frame format and 
enabling the Transmitter or the Receiver depending on the usage. 

The TXCn Flag can be used to check that the Transmitter has completed all transfers, 
and the RXC Flag can be used to check that there are no unread data in the receive buffer. 
Note that the TXCn Flag must be cleared before each transmission (before UDRn is
written) if it is used for this purpose

The MCU’s UART must be configured as 115200 8N1: 
115200 bauds, 8 bits de données, aucune parité (N) et 1 bit de stop (8N1).

Le baud rate se définit via UBRRn (USART Baud Rate Register), 
selon la formule :
UBRR=(FCPU/16×Baud)−1
Avec :
    F_CPU = 16 000 000 Hz (16 MHz)
    Baud rate = 115200
UBRR=(16000000/16×115200)−1=8
Donc :
    UBRR0L = 8 (partie basse)
    UBRR0H = 0 (partie haute)

1 start bit
5, 6, 7, 8, or 9 data bits
no, even or odd parity bit
1 or 2 stop bits
/ U2Xn (Universal Double Speed) dans le registre UCSRnA: 0 = normal, 1 = double

USART Baud Rate Registers = UBRR - page 204 section 20.11.5
UBRRnL - bits de 0 a 7?
UBRRnH - bits de 8 a 15?
UBRR[11:0] This is a 12-bit register which contains the USART baud rate. The UBRRnH contains the four most significant
bits, and the UBRRnL contains the eight least significant bits of the USART baud rate. 
UBRR[15:12] reserved

Format des données (8 bits, 1 stop, pas de parité → 8N1) → UCSR0C
USART Control and Status Register n C = UCSRnc page 202 section 20.11.4
Bit 0 – UCPOLn: Clock Polarity - for synchronous mode only
Bit 2:1 – UCSZn1:0: Character Size, combined with the UCSZn2 bit in UCSRnB sets the number of data bits in a frame
Bit 3 – USBSn: Stop Bit Select, This bit selects the number of stop bits to be inserted by the Transmitter. The Receiver ignores this setting.
Bits 5:4 – UPMn1:0: Parity Mode
Bits 7:6 – UMSELn1:0 USART Mode Select 00 for asynchronous

Activation de la transmission (TX) et réception (RX) → UCSR0B
UCSRnB – USART Control and Status Register n B - page 201 section 20.11.3
Bit	Nom	Description
7	RXCIE0	Interruption de réception : Active l'interruption quand des données sont reçues.
6	TXCIE0	Interruption de fin de transmission : Active l'interruption après qu'une transmission de données est terminée.
5	UDRIE0	Interruption de buffer vide : Active l'interruption quand le buffer de transmission (UDR) est vide, prêt à recevoir de nouvelles données.
4	RXEN0	Activation de la réception : Active le récepteur pour recevoir des données.
3	TXEN0	Activation de la transmission : Active le transmetteur pour envoyer des données.
2	UCSZ02	Taille des données (9 bits) : Lorsque ce bit est activé (1), l'UART fonctionne en mode 9 bits.
1	RXB8	Bit de données reçues 8ème (9 bits de données) : Utilisé pour les données 9 bits, ce bit contient le 9e bit lors de la réception.
0	TXB8	Bit de données transmises 8ème (9 bits de données) : Utilisé pour les données 9 bits, ce bit contient le 9e bit lors de la transmission.
 Le bit UDRE0 dans le registre UCSR0A indique si le buffer de transmission (UDR0) 
    est prêt à recevoir un nouveau caractère.
    UCSR0A est un registre qui contient plusieurs bits d'état et de contrôle pour l'UART, 
    et UDRE0 est un bit qui devient 1 lorsque le buffer de transmission est vide, 
    c'est-à-dire prêt à recevoir de nouvelles données.
    while attend que UDRE0 passe a 1
UCSRnA contient des drapeaux (flags) de statut et des paramètres de contrôle spécifiques pour la communication USART. 
UCSRnB principalement utilisé pour activer et configurer les différents modes de l'USART, ainsi que pour gérer les interruptions associées à la réception et la transmission des données.
RXCIE0 (bit 7) : L'interruption de réception. 
Si ce bit est activé (1), une interruption sera générée lorsque le drapeau RXC0 du UCSRnA devient actif (c'est-à-dire qu'un nouveau caractère a été reçu).
    UDR0 (USART Data Register 0) est un registre spécial pour transmettre et recevoir des données via le module USART
*/

#define BAUD 115200

void uart_init(){ //initializes the UART
    int ubrr = (F_CPU / (16 * BAUD) - 1);

    UBRR0H = ((ubrr + 1 ) >> 8);        // includes the 8 most significant bits of UBRR into the Register = 0 
    UBRR0L = (ubrr + 1 );               // includes the 8 less significant bits of UBRR into the Register = 8

    UCSR0B = (1<<TXEN0)| (1 << RXEN0) | (1 << RXCIE0); // Enable transmitter and recepter + flag for interruption
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

char uart_rx(void){
    while ( !(UCSR0A & (1<<RXC0)));     //Wait for data
    return UDR0;
}


void uart_tx(char c){ //writes a character to the PC serial port / code frin 20.6.1 page 186
    while (!(UCSR0A & (1 << UDRE0)));   // Attendre que le buffer soit prêt
   
    UDR0 = c;                           // Envoyer le caractère
}

char echo(char c){
    if (c >= 65 && c <= 90)
        c += 32;
    else if (c >= 97 && c <= 122)
        c -= 32;
    return c;
}

ISR(USART_RX_vect){
    char received_char = UDR0;
    uart_tx(echo(received_char));
}

int main(){
    uart_init();
    sei();                              // Active les interruptions globales

    while(1){}
    return 0;
}