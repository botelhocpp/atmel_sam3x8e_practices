#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <stdint.h>
#include <stdbool.h>
#include <sam3x8e.h>

/* Data Types */

typedef struct axis_t {
    int16_t x;
    int16_t y;
    int16_t z;  
} axis_t;

/* Public Macros */

#define MPU6050_MIN_RANGE         (-17000)
#define MPU6050_MAX_RANGE         (+17000)

/* Public Functions */

bool ACCEL_Init(Twi *twi);

bool ACCEL_CheckDevice(Twi *twi);

bool ACCEL_Reset(Twi *twi);

bool ACCEL_GetTemperature(Twi *twi, float *temperature);

bool ACCEL_GetAngles(Twi *twi, axis_t *axis);

bool ACCEL_GetRawAxis(Twi *twi, axis_t *axis);

bool ACCEL_GetRawX(Twi *twi, int16_t *x_value);

bool ACCEL_GetRawY(Twi *twi, int16_t *y_value);

bool ACCEL_GetRawZ(Twi *twi, int16_t *z_value);

bool ACCEL_IsAsleep(Twi *twi);

bool ACCEL_EnableSleepMode(Twi *twi);

bool ACCEL_DisableSleepMode(Twi *twi);

#endif
