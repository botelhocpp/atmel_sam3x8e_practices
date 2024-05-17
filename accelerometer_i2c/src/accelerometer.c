#include "accelerometer.h"

#include "twi.h"
#include "utils.h"

#define MPU6050_ADDRESS                   (0x68)
#define	MPU6050_PWR_MGMT_1_REG		        (0x6B)
#define MPU6050_WHO_AM_I_REG              (0x75)
#define	MPU6050_CONFIG_REG		            (0x1A)
#define	MPU6050_ACCEL_CONFIG_REG	        (0x1C)
#define	MPU6050_ACCEL_XOUT_H_REG	        (0x3B)	
#define	MPU6050_ACCEL_XOUT_L_REG	        (0x3C)
#define	MPU6050_ACCEL_YOUT_H_REG	        (0x3D)
#define	MPU6050_ACCEL_YOUT_L_REG	        (0x3E)
#define	MPU6050_ACCEL_ZOUT_H_REG	        (0x3F)
#define	MPU6050_ACCEL_ZOUT_L_REG	        (0x40)
#define	MPU6050_TEMP_OUT_H_REG		        (0x41)
#define	MPU6050_TEMP_OUT_L_REG		        (0x42)

#define MPU6050_ACCEL_SCALE               (16384.0F)
#define MPU6050_PWR_MGMT_1_SLEEP          (1U << 6)
#define MPU6050_CALC_TEMP(_temp)           (_temp/340.0 + 36.53)
  
bool ACCEL_Init(Twi *twi) {
    const uint8_t pwr_mgmt_1_reg_value = 0x00;
    const uint8_t config_reg_value = 0x01;
    const uint8_t accel_config_reg_value = 0x00;

    /* Wake-up MPU6050 */
    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    /* Set a Minimal Low-Pass Filter */
    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_CONFIG_REG, 1, &config_reg_value, 1)) {
      return false;
    }

    /* Set Full Scale Range to ±2g */
    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG_REG, 1, &accel_config_reg_value, 1)) {
      return false;
    }

    return true;
}

bool ACCEL_CheckDevice(Twi *twi) {
    uint8_t who_am_i_reg_value;
    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_WHO_AM_I_REG, 1, &who_am_i_reg_value, 1)) {
      return false;
    }
    return who_am_i_reg_value == MPU6050_ADDRESS;
}

bool ACCEL_Reset(Twi *twi) {
    const uint8_t pwr_mgmt_1_reg_value = 0x80;

    /* Reset MPU6050 */
    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    return true;
}

bool ACCEL_GetTemperature(Twi *twi, float *temperature) {
    uint8_t temp_reg_value[2];

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_TEMP_OUT_H_REG, 1, temp_reg_value, 2)) {
        return false;
    }

    int16_t temperature_raw = (temp_reg_value[0] << 8) | temp_reg_value[1];

    *temperature = MPU6050_CALC_TEMP(temperature_raw);

    return true;
}

bool ACCEL_GetAngles(Twi *twi, axis_t *axis) {

  if(!ACCEL_GetRawAxis(twi, axis)) {
    return false;
  }
  
  axis->x = map(axis->x, MPU6050_MIN_RANGE, MPU6050_MAX_RANGE, 0, 180);
  axis->y = map(axis->y, MPU6050_MIN_RANGE, MPU6050_MAX_RANGE, 0, 180);
  axis->z = map(axis->z, MPU6050_MIN_RANGE, MPU6050_MAX_RANGE, 0, 180); 

  return true;
}

bool ACCEL_GetRawAxis(Twi *twi, axis_t *axis) {
    uint8_t axis_reg_value[6];

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H_REG, 1, axis_reg_value, 6)) {
        return false;
    }

    axis->x = (axis_reg_value[0] << 8) | axis_reg_value[1];
    axis->y = (axis_reg_value[2] << 8) | axis_reg_value[3];
    axis->z = (axis_reg_value[4] << 8) | axis_reg_value[5];

    return true;
}

bool ACCEL_GetRawX(Twi *twi, int16_t *x_value) {
    uint8_t xout_reg_value[2];

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H_REG, 1, xout_reg_value, 2)) {
        return false;
    }
    
    *x_value = (xout_reg_value[0] << 8) | xout_reg_value[1];

    return true;
}

bool ACCEL_GetRawY(Twi *twi, int16_t *y_value) {
    uint8_t yout_reg_value[2];

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_ACCEL_YOUT_H_REG, 1, yout_reg_value, 2)) {
        return false;
    }

    *y_value = (yout_reg_value[0] << 8) | yout_reg_value[1];

    return true;
}

bool ACCEL_GetRawZ(Twi *twi, int16_t *z_value) {
    uint8_t zout_reg_value[2];

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT_H_REG, 1, zout_reg_value, 2)) {
        return false;
    }

    *z_value = (zout_reg_value[0] << 8) | zout_reg_value[1];

    return true;

}

bool ACCEL_IsAsleep(Twi *twi) {
    uint8_t pwr_mgmt_1_reg_value;

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    return (pwr_mgmt_1_reg_value & MPU6050_PWR_MGMT_1_SLEEP) == MPU6050_PWR_MGMT_1_SLEEP;
}

bool ACCEL_EnableSleepMode(Twi *twi) {
    uint8_t pwr_mgmt_1_reg_value;

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    pwr_mgmt_1_reg_value |= MPU6050_PWR_MGMT_1_SLEEP;

    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    return true;
}

bool ACCEL_DisableSleepMode(Twi *twi) {
    uint8_t pwr_mgmt_1_reg_value;

    if(!TWI_ReadData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    pwr_mgmt_1_reg_value &= ~MPU6050_PWR_MGMT_1_SLEEP;

    if(!TWI_WriteData(twi, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1_REG, 1, &pwr_mgmt_1_reg_value, 1)) {
      return false;
    }

    return true;
}
