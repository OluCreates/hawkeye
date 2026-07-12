#pragma once
#include <stdint.h>

void uart_init(uint32_t baud);
void uart_print(const char* str);
void uart_print_hex(uint8_t val);