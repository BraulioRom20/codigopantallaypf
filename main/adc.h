#pragma once
#include "driver/adc.h"


#define VOLTAGE_PIN ADC1_CHANNEL_4  // GPIO32 → ADC1_CH4
#define CURRENT_PIN ADC1_CHANNEL_0  // GPIO36 → ADC1_CH0

#define VOLTAGE_DIVIDER_RATIO 2.0f  // (R1+R2)/R2
#define ACS712_SENSITIVITY 0.066f    // 66mV/A para ACS712-30A

void adc_init(void);
float read_voltage(void);
float read_current(void);