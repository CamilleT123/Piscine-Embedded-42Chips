#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"

/*RX = PD0 et TX = PD1 - written on card

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
U2Xn (Universal Double Speed) dans le registre UCSRnA: 0 = normal, 1 = double

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

set le timer : 
     choisir le mode, fastPWM ou CTC ou autre
     choisir le prescaler
choisir CTC
pour prescaler 1024: CS12 + CS10 (Table 16-5 page 143)
toutes les 2 sec donc frequence de 0.5Hz
si prescaler = 1024, OCR1A = 31249
TIMSK1 Timer/Counter1 Interrupt Mask Register section 16.11.8 page 144
table 12-1 page 66 defines the interrupt vectors
*/
#define BAUD 115200

void uart_init(){ //initializes the UART
    int ubrr = (F_CPU / (16 * BAUD) - 1);

    UBRR0H = ((ubrr + 1 ) >> 8);                // includes the 8 most significant bits of UBRR into the Register = 0 
    UBRR0L = (ubrr + 1 );                       // includes the 8 less significant bits of UBRR into the Register = 8

    UCSR0B = (1<<TXEN0);                    // Enable transmitter and recepter
    
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // pour  8 bits, pas de parité, 1 bit de stop (8N1) :

}

void uart_tx(char c){ //writes a character to the PC serial port / code frin 20.6.1 page 186
    while (!(UCSR0A & (1 << UDRE0)));       // Attendre que le buffer soit prêt
    UDR0 = c;                               // Envoyer le caractère
}

void uart_printstr(const char* str){
    while(*str){
        uart_tx(*str);
        str++;
    }
}

void timer_init(){
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    OCR1A = (31249);                        
    TIMSK1 |= (1 << OCIE1A);                // Active l'interruption du timer sur la correspondance OCR1A
    sei();                                  // Active les interruptions globales
}

ISR(TIMER1_COMPA_vect){
    uart_printstr("Hello World!\r\n");
}

int main(){
    uart_init();
    timer_init();
    
    while(1){}
    return 0;
}