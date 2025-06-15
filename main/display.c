#include "display.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "string.h"

#define TAG "DISPLAY"


#define PIN_SPI_MOSI 21
#define PIN_SPI_CLK  28
#define PIN_SPI_CS   32
#define PIN_SPI_DC   34
#define PIN_SPI_RST  35


#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

spi_device_handle_t spi;
static uint16_t* frame_buffer = NULL;

#define CMD_SWRESET 0x01
#define CMD_SLPOUT  0x11
#define CMD_DISPON  0x29
#define CMD_CASET   0x2A
#define CMD_RASET   0x2B
#define CMD_RAMWR   0x2C

// comandos
static void send_cmd(uint8_t cmd) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
        .user = (void*)0
    };
    
    gpio_set_level(PIN_SPI_DC, 0);
    ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);
}

// datos
static void send_data(uint8_t *data, uint16_t len) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data,
        .user = (void*)1
    };
    
    gpio_set_level(PIN_SPI_DC, 1);
    ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);
}

// dibujo
static void set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint8_t data[4];
    
    send_cmd(CMD_CASET);
    data[0] = x1 >> 8; data[1] = x1 & 0xFF;
    data[2] = x2 >> 8; data[3] = x2 & 0xFF;
    send_data(data, 4);
    
    send_cmd(CMD_RASET);
    data[0] = y1 >> 8; data[1] = y1 & 0xFF;
    data[2] = y2 >> 8; data[3] = y2 & 0xFF;
    send_data(data, 4);
}

void display_init(void) {
    // SPI
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = PIN_SPI_MOSI,
        .sclk_io_num = PIN_SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * 2,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 26 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_SPI_CS,
        .queue_size = 7,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    // SPI
    ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, &devcfg, &spi));

    // pines
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_SPI_DC) | (1ULL << PIN_SPI_RST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // reset
    gpio_set_level(PIN_SPI_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_SPI_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    // secuencia para iniciar
    send_cmd(CMD_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(120));
    send_cmd(CMD_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(120));
    send_cmd(CMD_DISPON);
    vTaskDelay(pdMS_TO_TICKS(120));

    // framebuffer
    frame_buffer = (uint16_t*)heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA);
    assert(frame_buffer != NULL);

    ESP_LOGI(TAG, "Display inicializado. Tamaño: %dx%d", DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void display_clear(uint16_t color) {
    for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        frame_buffer[i] = color;
    }
}

void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
        frame_buffer[y * DISPLAY_WIDTH + x] = color;
    }
}

void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    display_draw_pixel(x0, y0 + r, color);
    display_draw_pixel(x0, y0 - r, color);
    display_draw_pixel(x0 + r, y0, color);
    display_draw_pixel(x0 - r, y0, color);

    while(x < y) {
        if(f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display_draw_pixel(x0 + x, y0 + y, color);
        display_draw_pixel(x0 - x, y0 + y, color);
        display_draw_pixel(x0 + x, y0 - y, color);
        display_draw_pixel(x0 - x, y0 - y, color);
        display_draw_pixel(x0 + y, y0 + x, color);
        display_draw_pixel(x0 - y, y0 + x, color);
        display_draw_pixel(x0 + y, y0 - x, color);
        display_draw_pixel(x0 - y, y0 - x, color);
    }
}

void display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while(1) {
        display_draw_pixel(x1, y1, color);
        if(x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if(e2 >= dy) { err += dy; x1 += sx; }
        if(e2 <= dx) { err += dx; y1 += sy; }
    }
}

void display_draw_string(const char *text, uint16_t x, uint16_t y, uint16_t color, font_size_t size) {
    // implementación simple
    const uint8_t *font = get_font(size);
    uint16_t font_width = get_font_width(size);
    uint16_t font_height = get_font_height(size);
    
    while(*text) {
        draw_char(*text, x, y, color, font, font_width, font_height);
        x += font_width;
        text++;
    }
}

void display_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    set_window(x, y, x + w - 1, y + h - 1);
    send_cmd(CMD_RAMWR);
    send_data((uint8_t*)bitmap, w * h * 2);
}

void display_start_frame(void) {
    // sincronización si es necesaria
}

void display_end_frame(void) {
    // framebuffer al display
    set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
    send_cmd(CMD_RAMWR);
    send_data((uint8_t*)frame_buffer, DISPLAY_WIDTH * DISPLAY_HEIGHT * 2);
}

void display_set_brightness(uint8_t percent) {
    // no se como es el display
    uint8_t brightness = (percent * 255) / 100;
    send_cmd(0x51); // a modificar dependiendo de como quede
    send_data(&brightness, 1);
}