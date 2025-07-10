#pragma once
#include "esp_err.h"
#include "driver/i2c.h"

#define MPU6050_I2C_ADDR         0x68
#define MPU6050_WHO_AM_I_REG     0x75
#define MPU6050_PWR_MGMT_1_REG   0x6B
#define MPU6050_ACCEL_XOUT_H     0x3B
#define MPU6050_GYRO_XOUT_H      0x43

typedef struct {
    int16_t ax;
    int16_t ay; 
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
}mpu6050_data_t;

esp_err_t mpu6050_init(i2c_port_t i2c_num);
esp_err_t mpu6050_read(mpu6050_data_t *data, i2c_port_t i2c_num);
esp_err_t mpu6050_read_bytes(i2c_port_t i2c_num, uint8_t reg_addr, uint8_t *data, size_t len);
esp_err_t mpu6050_write_byte(i2c_port_t i2c_num, uint8_t reg_addr, uint8_t data);

