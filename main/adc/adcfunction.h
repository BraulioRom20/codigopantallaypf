#pragma once
//#include "esp_adc/adc_oneshot.h" // Nueva ubicacion
//no necesario el adc_oneshot porque leo por calibracion.

// canales adc
#define VOLTAGE_ADC_CHANNEL ADC_CHANNEL_4  // GPIO32 (ADC1)
#define CURRENT_ADC_CHANNEL ADC_CHANNEL_0  // GPIO36 (ADC1)

// factores de conversión
#define VOLTAGE_DIVIDER_RATIO 11.0f  // (R1+R2)/R2 *(no me acuerdo las resistencia que iban aca)
#define ACS712_SENSITIVITY 0.066f    // 66mV/A para ACS712-30A

void adc_init(void);
float read_voltage(void);
float read_current(void);