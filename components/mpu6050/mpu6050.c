#include "mpu6050.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL ESP_LOG_VERBOSE
#endif

static const char *TAG = "MPU6050";


//I2c 
void mpu6050_init_i2c() {
// 1. Configuración I2C
    ESP_LOGI(TAG, "Configurando I2C...");
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,    // GPIO21 para ESP32 estándar
        .scl_io_num = 22,    // GPIO22 para ESP32 estándar
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000, // 100 kHz (inicial)
    };

    // 2. Inicialización I2C
    esp_err_t ret = i2c_param_config(I2C_NUM_0, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error configurando I2C: %s", esp_err_to_name(ret));
        //return ESP_E;
    }

    ret = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error instalando driver I2C: %s", esp_err_to_name(ret));
        //return 0;
    }

}


esp_err_t mpu6050_write_byte(i2c_port_t i2c_num, uint8_t reg_addr, uint8_t data) {
    uint8_t write_buf[2] = {reg_addr, data};
    return i2c_master_write_to_device(i2c_num, MPU6050_I2C_ADDR, write_buf, sizeof(write_buf), 100);
}

esp_err_t mpu6050_read_bytes(i2c_port_t i2c_num, uint8_t reg_addr, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(i2c_num, MPU6050_I2C_ADDR, &reg_addr, 1, data, len, 100);
}

esp_err_t mpu6050_init(i2c_port_t i2c_num) {
    uint8_t who_am_i;
    esp_err_t ret = mpu6050_read_bytes(I2C_NUM_0, MPU6050_WHO_AM_I_REG, &who_am_i, 1);

    if (ret == ESP_OK) {
        if (who_am_i != MPU6050_I2C_ADDR) {
            ESP_LOGE(TAG, "WHO_AM_I incorrecto (Leído: 0x%02X, Esperado: 0x%02X)", 
                    who_am_i, MPU6050_I2C_ADDR);
            // Diagnóstico adicional
            if (who_am_i == 0x00 || who_am_i == 0xFF) {
                ESP_LOGE(TAG, "Posible fallo de conexión o alimentación");
            } else {
                ESP_LOGE(TAG, "Posible dirección I2C incorrecta (prueba con 0x%02X)", 
                        who_am_i);
            }
            return ESP_FAIL;
        }
    } else {
        ESP_LOGE(TAG, "Fallo al leer WHO_AM_I: %s", esp_err_to_name(ret));
        return ret;
    }
        // Wake up MPU6050
        uint8_t cmd[] = { MPU6050_PWR_MGMT_1_REG, 0x00 };
        ret = i2c_master_write_to_device(i2c_num, MPU6050_I2C_ADDR, cmd, 2, 100);
        return ret;
 
}

esp_err_t mpu6050_read(mpu6050_data_t *data, i2c_port_t i2c_num) {
    uint8_t buffer[14];
    esp_err_t ret = mpu6050_read_bytes(i2c_num, MPU6050_ACCEL_XOUT_H, buffer, 14);

    if(ret != ESP_OK){
        ESP_LOGE(TAG, "Error de lectura %d", ret);
        return ret;
    }

    data->ax = (int16_t)(buffer[0] << 8 | buffer[1]);
    data->ay = (int16_t)(buffer[2] << 8 | buffer[3]);
    data->az = (int16_t)(buffer[4] << 8 | buffer[5]);

    data->gx = (int16_t)(buffer[8] << 8 | buffer[9]);
    data->gy = (int16_t)(buffer[10] << 8 | buffer[11]);
    data->gz = (int16_t)(buffer[12] << 8 | buffer[13]);


        // Mostrar valores en el monitor serial
    ESP_LOGI(TAG, "Acelerómetro - X: %d, Y: %d, Z: %d", data->ax, data->ay, data->az);
    ESP_LOGI(TAG, "Giroscopio - X: %d, Y: %d, Z: %d", data->gx, data->gy, data->gz);
    return ESP_OK;
}
