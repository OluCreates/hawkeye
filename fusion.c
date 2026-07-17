#include "fusion.h"
#include "uart.h"
#include <stdint.h>
#define ALPHA 98
#define LIGHT_THRESHOLD 10
#define TEMP_THRESHOLD  40
static int16_t tilt_angle = 0;
void fusion_init(){
    tilt_angle = 0;
}

int16_t complementary_filter(int16_t accel_x, int16_t accel_y, int16_t gyro_x, uint16_t dt){
    (void)accel_y;
   int16_t gyro_contribution = tilt_angle + (gyro_x * dt / 1000);
   int16_t accel_angle = accel_x * 90 / 16384;
   tilt_angle = (98 * gyro_contribution / 100) + (2 * accel_angle / 100);
   return tilt_angle;
}

uint8_t light_direction(uint16_t leftLight, uint16_t rightLight){
 int16_t diff = leftLight - rightLight;
 uint8_t angle = 90 - (diff * 90 / 255);
 return angle;
}
uint8_t tilt_compensate(uint8_t light_angle, int16_t tilt_angle){
 int16_t result = light_angle - (tilt_angle / 100);
 if (result < 0) result = 0;
 if (result > 180) result = 180;
 return (uint8_t)result;
}
uint8_t confidence_score(uint16_t leftLight, uint16_t rightLight, int16_t tilt_angle){
    (void) tilt_angle;
  int16_t diff = abs(leftLight - rightLight);
   return diff * 100 / 255;
}
const char* classify_event(uint16_t leftLight, uint16_t rightLight, uint16_t temp){
    char* status = "SCANNING...";
 if(temp < TEMP_THRESHOLD && leftLight < LIGHT_THRESHOLD && rightLight < LIGHT_THRESHOLD ){
    status = "SCANNING...";

 } else if(temp > TEMP_THRESHOLD && leftLight < LIGHT_THRESHOLD && rightLight < LIGHT_THRESHOLD ){
    status = "HEAT SOURCE";
    
 } else if(temp < TEMP_THRESHOLD && abs((int16_t)leftLight - (int16_t)rightLight) > LIGHT_THRESHOLD ){
    status = "LIGHT SOURCE";
 
 } else if(temp > TEMP_THRESHOLD && abs((int16_t)leftLight - (int16_t)rightLight) > LIGHT_THRESHOLD){
    status = "COMBINED EVENT";

 } 
    return status;
}
uint8_t fusion_update(int16_t accel_x, int16_t accel_y, int16_t gyro_x, uint16_t leftLight, uint16_t rightLight, uint16_t temp, uint16_t dt){
  char* label;
 int16_t tilt = complementary_filter(accel_x, accel_y, gyro_x, dt);
 uint8_t light_angle = light_direction(leftLight, rightLight);
 uint8_t compensated = tilt_compensate(light_angle, tilt);
 uint8_t confidence = confidence_score(leftLight, rightLight, tilt);
 label = classify_event( leftLight,  rightLight,  temp);
 uart_print(label);
 uart_print("Confidence Score: ");
 uart_print_hex((uint8_t)(confidence));
 return compensated;
}
