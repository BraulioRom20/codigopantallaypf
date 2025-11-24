

#ifndef NEO6M_H_
#define NEO6M_H_

#include <stdio.h>
#include <sys/_intsup.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "hal/ledc_types.h"
#include "portmacro.h"
#include "esp_log.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <sys/_intsup.h>
#include <unistd.h>
#include "driver/uart.h"
#include "esp_err.h"
#include "freertos/idf_additions.h"
#include "hal/uart_types.h"
#include "portmacro.h"
#include "esp_log.h"
#include "string.h"
#include "math.h"


#ifdef __cplusplus
extern "C" {
#endif
void gps_starting();
void raw_nmea(double *latitude, double *longitude,char *lat_hemisphere, char *lon_hemisphere,float *speedKmh);
#ifdef __cplusplus
}
#endif


#endif 
