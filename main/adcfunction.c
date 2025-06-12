#include "adcfunction.h"
#include "esp_log.h"
#include "sdkconfig.h" // Asegura que las macros de configuración estén disponibles
#include "esp_adc/adc_cali.h"      // calibración
#include "esp_adc/adc_cali_scheme.h" // esquemas de calibración 

#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL ESP_LOG_VERBOSE
#endif

static const char* TAG = "ADC";
static adc_oneshot_unit_handle_t adc1_handle;
static adc_cali_handle_t adc_cali_handle = NULL;

// calibracion
static bool adc_calibration_init(void) {
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    return (adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle) == ESP_OK);
}

void adc_init(void) {
    // config del adc
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &adc1_handle));

    // config de canales ADC
    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, VOLTAGE_ADC_CHANNEL, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, CURRENT_ADC_CHANNEL, &chan_config));

    // calibración del ADC
    if (!adc_calibration_init()) {
        ESP_LOGE(TAG, "Falló la calibración del ADC");
    }
}

float read_voltage(void) {
    // medición por oneshot
    int raw;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, VOLTAGE_ADC_CHANNEL, &raw));

    // voltaje calibrado en mV
    int voltage_mv;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, raw, &voltage_mv));

    // factor del divisor resistivo
    return (voltage_mv / 1000.0f) * VOLTAGE_DIVIDER_RATIO;  // Convierte a Voltios
}

float read_current(void) {
    // medición por oneshot
    int raw;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, CURRENT_ADC_CHANNEL, &raw));

    // Voltaje calibrado en mV
    int voltage_mv;
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, raw, &voltage_mv));
    float voltage_v = voltage_mv / 1000.0f;

    // fórmula para ACS712 (1.65V = 0A, 66mV/A para acs de 30A)
    return (voltage_v - 1.65f) / ACS712_SENSITIVITY;
}