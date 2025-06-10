#include "adc.h"
#include "esp_log.h"

static const char* TAG = "ADC";

void adc_init(void) {
    // adc1
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(VOLTAGE_PIN, ADC_ATTEN_DB_11);  // me perdi con los pines auxilio
    adc1_config_channel_atten(CURRENT_PIN, ADC_ATTEN_DB_11);  // x2
}

float read_voltage(void) {
    int raw = adc1_get_raw(VOLTAGE_PIN);  // x3
    return (raw * 3.3f / 4095.0f) * VOLTAGE_DIVIDER_RATIO;
}

float read_current(void) {
    int raw = adc1_get_raw(CURRENT_PIN);  // x4
    float voltage = (raw * 3.3f / 4095.0f);
    return (voltage - 1.65f) / ACS712_SENSITIVITY;  // 1.65V = 0A no se ni que hice acá
}