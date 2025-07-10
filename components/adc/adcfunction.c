#include "adcfunction.h"


// calibracion init
static bool adc_calibration_init(void) {
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
        .default_vref = DEFAULT_VREF,
    };
    return (adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle) == ESP_OK);
}

void adc_init(void) {
    
    
    // config del adc oneshot 
    adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &adc_oneshot_handle));

    // config de canales ADC
    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };  


    // calibración del ADC
    if (!adc_calibration_init()) {
        ESP_LOGE(TAG, "Falló la calibración del ADC");
    }
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_oneshot_handle, VOLTAGE_ADC_CHANNEL, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_oneshot_handle, CURRENT_ADC_CHANNEL, &chan_config));
}


float read_voltage(void) {
    // medición por oneshot
    voltage_mv =0; int raw = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc_oneshot_handle, VOLTAGE_ADC_CHANNEL, &raw));

    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, raw, &voltage_mv));

    // factor del divisor resistivo
    return (voltage_mv / 1000.0f) * VOLTAGE_DIVIDER_RATIO;  // Convierte a Voltios
}

float read_current(void) {
    // medición por oneshot

    // medicion de corriente calibrada en mV
    current_acs = 0; int raw = 0;
    
    ESP_ERROR_CHECK(adc_oneshot_read(adc_oneshot_handle, CURRENT_ADC_CHANNEL, &raw));
    
    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, raw, &current_acs));
    current_acs = current_acs / 1000.0f;

    // fórmula para ACS712 (1.65V = 0A, 66mV/A para acs de 30A)
    return (current_acs - 1.65f) / ACS712_SENSITIVITY;
}