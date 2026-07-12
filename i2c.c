#include "i2c.h"
#include <avr/io.h>

void i2c_init(void) {
    TWBR = 72; //  set I2C clock speed 
}
void i2c_start(void) {
    TWCR = (1 << TWSTA) | (1 << TWEN)| (1 << TWINT); // send start condition
    while (!(TWCR & (1 << TWINT))); // wait for start condition to be transmitted
}
void i2c_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN)| (1 << TWINT); // send stop condition
    while (TWCR & (1 << TWSTO)); // wait for stop condition to be transmitted
}
void i2c_write(uint8_t data) {
    TWDR = data; // load data into data register
    TWCR = (1 << TWEN)| (1 << TWINT); // start transmission
    while (!(TWCR & (1 << TWINT))); // wait for transmission to complete
  
}
uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA); // enable ACK
    while (!(TWCR & (1 << TWINT))); // wait for data to be received
    return TWDR; // return received data
}
uint8_t i2c_read_nack(void){
     TWCR = (1 << TWEN) | (1 << TWINT); // do not enable ACK
    while (!(TWCR & (1 << TWINT))); // wait for data to be received
    return TWDR; // return received data
}
// writing value to slave
void i2c_write_reg(uint8_t addr, uint8_t reg, uint8_t data){
    i2c_start();
    i2c_write(addr << 1);
    i2c_write(reg);
    i2c_write(data);
    i2c_stop();
}
  //reading value from slave
uint8_t i2c_read_reg(uint8_t addr, uint8_t reg){
    i2c_start();
    i2c_write(addr << 1);
    i2c_write(reg);
    i2c_start();
    i2c_write((addr << 1) |1);
    uint8_t x = i2c_read_nack();
    i2c_stop();
    return x;
}