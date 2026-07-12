#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

  //accel init
    uint8_t xlow =0;
    uint8_t xhigh = 0;
    uint8_t ylow =0;
    uint8_t yhigh = 0;
    uint8_t zlow =0;
    uint8_t zhigh = 0;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
void accelerationMeasure();


int main(){
    uint8_t test;
    i2c_init();
    _delay_ms(100);
    i2c_write_reg(0x68, 0x6B, 0x00);
    
    while(1){
         accelerationMeasure();
        test = i2c_read_reg(0x68, 0x75);
        uart_print("WHO_AM_I: ");
        uart_print_hex(test);
        _delay_ms(500);
    }
}

void accelerationMeasure(){
  xlow = i2c_read_reg(0x68, 0x3C);
        xhigh = i2c_read_reg(0x68, 0x3B);
         ylow = i2c_read_reg(0x68, 0x3E);
        yhigh = i2c_read_reg(0x68, 0x3D);
         zlow = i2c_read_reg(0x68, 0x40);
        zhigh = i2c_read_reg(0x68, 0x3F);
        accel_x = (xhigh << 8) | xlow;
        accel_y = (yhigh << 8) | ylow;
        accel_z = (zhigh << 8) | zlow;
        uart_print("X high: ");
        uart_print_hex(xhigh);
        uart_print("X low: ");
        uart_print_hex(xlow);
        uart_print("Y high: ");
        uart_print_hex(yhigh);
        uart_print("Y low: ");
        uart_print_hex(ylow);
        uart_print("Z high: ");
        uart_print_hex(zhigh);
        uart_print("Z low: ");
        uart_print_hex(zlow);
}