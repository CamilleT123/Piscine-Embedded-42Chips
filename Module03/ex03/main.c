#ifndef __AVR_ATmega328P__
# define __AVR_ATmega328P__
#endif

#include "avr/io.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include <stddef.h>

#define BAUD 115200

int i = 0;
char buf[7] = {'0', '0', '0', '0', '0', '0', '\0'};

void uart_init(){                                   //initializes the UART
    int ubrr = (F_CPU / (16 * BAUD) - 1);

    UBRR0H = ((ubrr + 1 ) >> 8);                    // includes the 8 most significant bits of UBRR into the Register = 0 
    UBRR0L = (ubrr + 1 );                           // includes the 8 less significant bits of UBRR into the Register = 8

    UCSR0B = (1<<TXEN0)| (1 << RXEN0) | (1 << RXCIE0); // Enable transmitter and recepter + flag for interruption
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c){                               //writes a character to the PC serial port / code from 20.6.1 page 186
    while (!(UCSR0A & (1 << UDRE0)));               // Attendre que le buffer soit prêt
    UDR0 = c;                                       // Envoyer le caractère
}

void init_rgb(){                                    //initializes the timers
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);   // as output

    TCCR0A = (1 << WGM00) | (1 << WGM01);           // sets FastPWM
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1);        // Set OC0A and OC0B on Compare Match, set OC0B at BOTTOM (non-inverting mode).
    TCCR0B = (1 << CS01);                           // no prescaler

    TCCR2A = (1 << WGM21) | (1 << WGM20);
    TCCR2A |= (1 << COM2B1) ;
    TCCR2B = (1 << CS20);                           // no prescaler (Table 16-5 page 143)
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){      // set the color of the LED
   OCR0A = g;
   OCR0B = r;
   OCR2B = b;
}

int is_hex(char c){
    if ((c >= 48 && c <= 57) || (c >= 65 && c<= 70) || (c >= 97 && c<=102))
    return 1;
    return 0;
}

int ft_isdigit(char c) {
    return c >= '0' && c <= '9';
}


uint8_t ft_hexa_to_deci(char *str) {
    int result = 0;

    while (*str) {
        char c = *str;
        int digit;

        if (ft_isdigit(c)) {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            break;
        }

        if (digit >= 16) {
            break;
        }

        result = result * 16 + digit;
        str++;
    }
    return result;
}

void uart_printstr(const char* str){
    while(*str){
        uart_tx(*str);
        str++;
    }
}

void turn_on_light(char *buf){
    char r[3] = {'\0', '\0', '\0'};
    char g[3] = {'\0', '\0', '\0'};
    char b[3] = {'\0', '\0', '\0'};
    r[0] = buf[0];
    r[1] = buf[1];
    uint8_t red = ft_hexa_to_deci(r);

    g[0] = buf[2];
    g[1] = buf[3];
    uint8_t green = ft_hexa_to_deci(g);

    b[0] = buf[4];
    b[1] = buf[5];
    uint8_t blue = ft_hexa_to_deci(b);

    set_rgb(red, green, blue);
}

void reinit(){
    uart_printstr("\r\nPlease enter an HEX RGB: ");
    i = 0;
    for (int j = 0; j < 6; j++)
        buf[j] = '0';
}

ISR(USART_RX_vect){
    char received_char = UDR0;
    if (is_hex(received_char)){
        if (i < 6){
            buf[i] = received_char;
            uart_tx((received_char));
            i++;
        }
    }
    if(((received_char == '\n') || (received_char == '\r'))){
        uart_printstr("\r\nRequested color:");
        uart_printstr(buf);
        turn_on_light(buf);
        reinit();
    }
}

int main(){
    uart_init(); 
    init_rgb();
    uart_printstr("Please enter an HEX RGB: ");
    sei();                              
    while(1){}
    return 0;
}