#include "neo6m.h"
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


#define BUFFER (1024)
char buf[BUFFER];

static const char *TAG = "GPS";

double convert_to_decimal_degrees(double raw_coordinate) {
    // Extract the integer part as degrees.
    // For 3445.30788, floor(3445.30788 / 100) gives 34.
    double degrees = floor(raw_coordinate / 100.0);

    // Extract the fractional part (after removing degrees) as minutes.
    // For 3445.30788, fmod(3445.30788, 100.0) gives 45.30788.
    double minutes = fmod(raw_coordinate, 100.0);

    // Calculate the decimal degrees.
    // 34 + (45.30788 / 60)
    return degrees + (minutes / 60.0);
}

void gps_start(void)
{   
    const uart_port_t uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    
    uart_param_config(uart_num, &uart_config);
    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUFFER, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
}

void raw_nmea(void)
{
    

    memset(buf, 0, BUFFER);
    //len=0;
    //ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_2, (size_t*)&len));
    //ESP_LOGI(TAG, "LEN: %d", BUFFER);
    uart_read_bytes(UART_NUM_2, buf,BUFFER, portMAX_DELAY);
    ESP_LOGI(TAG, "Data: %s", buf);
    //data adquisiton latitude and longtitude
    char lat[12], lon[12];
    // Declare these new char variables to store 'S', 'W', 'N', or 'E'
    char lat_hemisphere; // Will store 'S' or 'N'
    char lon_hemisphere; // Will store 'W' or 'E'
    
    const char *start;

    start = strstr(buf, "$GPGGA");
    if (start != NULL) {
        //sscanf(start, "$GPGGA,%*f,%11[^,],%*c,%11[^,],%*c", lat, lon);
        sscanf(start, "$GPGGA,%*f,%11[^,],%c,%11[^,],%c", lat, &lat_hemisphere, lon, &lon_hemisphere);
        //GPGGA,002228.00,3445.30407,S,05816.68935,W,1,06,1.37,38.9,M,13.4,M,,*63
        ESP_LOGI(TAG,"lat: %s ^^ lon; %s",lat,lon);


        double latitude = atof(lat);
        double longitude = atof(lon);
        latitude = convert_to_decimal_degrees(latitude);
        longitude = convert_to_decimal_degrees(longitude);
        ESP_LOGI(TAG,"lat: %lf ^^ lon; %lf",latitude,longitude);
        ESP_LOGI(TAG,"lat: %c ^^ lon; %c",lat_hemisphere,lon_hemisphere);


        
    }

   vTaskDelay(1000);
}









void parse_nmea(const char *buf, char *latitude, char *longitude) {
    const char *start;
    char lat[12], lon[12];

  
    start = strstr(buf, "$GPGGA");
    if (start != NULL) {
        sscanf(start, "$GPGGA,%*f,%11[^,],%*c,%11[^,],%*c", lat, lon);
        sprintf(latitude, "%s", lat);
        sprintf(longitude, "%s", lon);
    }
}










void lat_long(void)
{
    
    char lat[12];
    char lon[12];
    void parse_nmea(const char *buf, char *lat, char *lon);
    ESP_LOGI(TAG, "Latitude is:%s\n", lat);
    ESP_LOGI(TAG, "Longitude is:%s\n", lon);
  }

void time(void)
{
    
    char time[20];
    sscanf(buf, "$GPRMC,%10[^,]", time); 
    ESP_LOGI(TAG, "Time is %s", time); 
    memset(time, 0, 20);
}

void speed_course(void)
{
    
    float speedKmh;
    float course;
    sscanf(buf, "$GPVTG,%*f,%*c,%*f,%*c,%*f,%*c,%f", &speedKmh); 
    sscanf(buf, "$GPVTG,%f", &course); 
    sscanf(buf, "$GPRMC,%*f,%*c,%*f,%*c,%*f,%*c,%*f,%f", &course); 
    ESP_LOGI(TAG, "Speed is %f", speedKmh); 
    ESP_LOGI(TAG, "Course is %f", course); 
   
   
}
