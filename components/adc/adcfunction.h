#pragma once

#include "sdkconfig.h" // Asegura que las macros de configuración estén disponibles
#include "esp_adc/adc_cali.h"      // calibración
#include "esp_adc/adc_cali_scheme.h" // esquemas de calibración 
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

// canales adc
#define VOLTAGE_ADC_CHANNEL ADC_CHANNEL_4  // GPIO32 (ADC1)
#define CURRENT_ADC_CHANNEL ADC_CHANNEL_0  // GPIO36 (ADC1)

//tension de ref
#define DEFAULT_VREF 1100

// factores de conversión
#define VOLTAGE_DIVIDER_RATIO 11.0f  // (R1+R2)/R2 *(no me acuerdo las resistencia que iban aca)
#define VOLTAGE_DIVIDER_RATIO_I 11.0f //CHANGE
#define SHUNT_RESOLUTION 0.075


#define TAG "ADC"

static adc_oneshot_unit_handle_t adc_oneshot_handle;
static adc_cali_handle_t adc_cali_handle = NULL;


int raw;

//variables for read voltage
int voltage_mv;

//variables for read current 
int current;


void adc_init(void); //inicializacion 
float read_voltage(void); //funcion para leer voltaje
float read_current(void); //funcion para leer corriente