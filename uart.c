#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

void uart_init(uint32_t baud) {
    uint16_t ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_print(const char* str) {
    while (*str) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *str++;
    }
}

void uart_print_hex(uint8_t val) {
    const char hex[] = "0123456789ABCDEF";
    uart_print("0x");
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = hex[val >> 4];
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = hex[val & 0x0F];
    uart_print("\r\n");
}