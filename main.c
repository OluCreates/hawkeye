#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
SemaphoreHandle_t xUARTMutex;

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
     
    //gyro init
    uint8_t gxlow =0;
    uint8_t gxhigh = 0;
    uint8_t gylow =0;
    uint8_t gyhigh = 0;
    uint8_t gzlow =0;
    uint8_t gzhigh = 0;
    int16_t gaccel_x;
    int16_t gaccel_y;
    int16_t gaccel_z;
void accelerationMeasure(void);
void gyroMeasure(void);
uint16_t adc_read(uint8_t channel);
void lightMeasure(void);
void tempMeasure(void);
void servo_init(void);
void servo_set(uint8_t angle);
void vTaskIMU(void * pvParameters);
void vTaskTemp (void *pvParameters);
void vTaskLight (void * pvParameters);
int main(){
    
    i2c_init();
    uart_init(9600);
    _delay_ms(100);
    i2c_write_reg(0x68, 0x6B, 0x00);
     servo_init();
    xUARTMutex = xSemaphoreCreateMutex();
    xTaskCreate(vTaskIMU, "IMU", 256, NULL, 2, NULL);
    xTaskCreate(vTaskLight, "Light", 128, NULL, 2, NULL);
    xTaskCreate(vTaskTemp, "Temp", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
        
  
}
  

void accelerationMeasure(void){
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

void gyroMeasure(void){
        gxlow = i2c_read_reg(0x68, 0x44);
        gxhigh = i2c_read_reg(0x68, 0x43);
        gylow = i2c_read_reg(0x68, 0x46);
        gyhigh = i2c_read_reg(0x68, 0x45);
        gzlow = i2c_read_reg(0x68, 0x48);
        gzhigh = i2c_read_reg(0x68, 0x47);
        gaccel_x = (gxhigh << 8) | gxlow;
        gaccel_y = (gyhigh << 8) | gylow;
        gaccel_z = (gzhigh << 8) | gzlow;
        uart_print("gX high: ");
        uart_print_hex(gxhigh);
        uart_print("gX low: ");
        uart_print_hex(gxlow);
        uart_print("gY high: ");
        uart_print_hex(gyhigh);
        uart_print("gY low: ");
        uart_print_hex(gylow);
        uart_print("gZ high: ");
        uart_print_hex(gzhigh);
        uart_print("gZ low: ");
        uart_print_hex(gzlow);
}

uint16_t adc_read(uint8_t channel) {
    ADMUX = (1 << REFS0) | (channel & 0x07);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void lightMeasure(void) {
    uint16_t leftLight = adc_read(0);  // A0
    uint16_t rightLight = adc_read(1); // A1
    uart_print("Left: ");
    uart_print_hex((uint8_t)(leftLight >> 2));
    uart_print("Right: ");
    uart_print_hex((uint8_t)(rightLight >> 2));
}

void tempMeasure(void){
    uint16_t temp = adc_read(2); // A2
    uart_print("Temp: ");
    uart_print_hex((uint8_t)(temp >> 2));
}
void servo_init(void) {
    // Fast PWM mode, non-inverting, ICR1 as top
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // prescaler 8
    ICR1 = 39999;   // 20ms period
    OCR1A = 3000;   // start at 90 degrees
    
    // Set pin 9 as output
    DDRB |= (1 << PB1);
}

void servo_set(uint8_t angle) {
    // map 0-180 degrees to 2000-4000 timer counts
    OCR1A = 2000 + ((uint32_t)angle * 2000 / 180);
}
void vTaskIMU(void * pvParameters){
  while(1){
    xSemaphoreTake(xUARTMutex, portMAX_DELAY);
     accelerationMeasure();
     gyroMeasure();
      xSemaphoreGive(xUARTMutex);
     vTaskDelay(pdMS_TO_TICKS(20)); // run every 20ms
  }
}
void vTaskTemp (void *pvParameters){
 while(1){
  xSemaphoreTake(xUARTMutex, portMAX_DELAY);
   tempMeasure();
    xSemaphoreGive(xUARTMutex);
  vTaskDelay(pdMS_TO_TICKS(1000));
 }
}
void vTaskLight (void * pvParameters){
   while(1){
    xSemaphoreTake(xUARTMutex, portMAX_DELAY);
    lightMeasure();
     xSemaphoreGive(xUARTMutex);
    vTaskDelay(pdMS_TO_TICKS(10));
}
}