//libreria hecha con spi para LoRA xl1278 esp-idf 5.4
#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//only pins of the lora module.
#define PIN_SPI_CS_LORA   5

#define PIN_SPI_RST_LORA  3
//#define PIN_SPI_MISO 33

//clk y mosi comparte con el bus del display tft 3.5


#ifdef __cplusplus
extern "C" {
#endif
//funciones lora:
void lora_init();
void lora_spi_init();
void lora_reset();


uint8_t lora_read_register(uint8_t reg);
void lora_write_register(uint8_t adress, uint8_t payload);
void lora_send_packet(const char *data);

//void show_lora_version();

#ifdef __cplusplus
}
#endif