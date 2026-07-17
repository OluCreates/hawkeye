#pragma once
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
void fusion_init(void);
int16_t complementary_filter(int16_t accel_x, int16_t accel_y, int16_t gyro_x, uint16_t dt);
uint8_t light_direction(uint16_t leftLight, uint16_t rightLight);
uint8_t tilt_compensate(uint8_t light_angle, int16_t tilt_angle);
uint8_t confidence_score(uint16_t leftLight, uint16_t rightLight, int16_t tilt_angle);
const char* classify_event(uint16_t leftLight, uint16_t rightLight, uint16_t temp);
uint8_t fusion_update(int16_t accel_x, int16_t accel_y, int16_t gyro_x, uint16_t leftLight, uint16_t rightLight, uint16_t temp, uint16_t dt);