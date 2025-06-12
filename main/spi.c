#include "spi.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_log_level.h"
#include "sdkconfig.h"
#include "driver/spi_master.h"

#ifndef CONFIG_LOG_MAXIMUM_LEVEL
#define CONFIG_LOG_MAXIMUM_LEVEL ESP_LOG_VERBOSE
#endif

static spi_device_handle_t spi_handle;
static const char* TAG = "SPI";

void spi_init(void) {
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = 23,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 26 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = 5,
        .queue_size = 7,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &devcfg, &spi_handle));
    
    ESP_LOGI(TAG, "SPI inicializado correctamente");
}

spi_device_handle_t get_spi_handle(void) {
    return spi_handle;
}