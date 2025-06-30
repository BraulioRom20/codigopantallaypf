#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// Tipos de fuente

#define TAG "DISPLAY"


#define PIN_SPI_MOSI 23
#define PIN_SPI_CLK  18
#define PIN_SPI_CS   5
#define PIN_SPI_DC   19
#define PIN_SPI_RST  17

// Tamaño del display
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 480

//comandos basicos 
#define CMD_SWRESET 0x01
#define CMD_SLPOUT  0x11
#define CMD_DISPON  0x29
#define CMD_CASET   0x2A
#define CMD_RASET   0x2B
#define CMD_RAMWR   0x2C

static uint8_t NORMAL_MODE[1] = {0x08};
//static uint8_t PIXEL_FORMAT_16BIT[1] = {0x55};
static uint8_t PIXEL_FORMAT_3BIT[1] = {0x01};

#define DMA_BUFFER_SIZE_PIXELS 4096 // Example: 4096 pixels
#define DMA_BUFFER_SIZE_BYTES (DMA_BUFFER_SIZE_PIXELS) // 1 bytes per pixel (8 color 3bit)
//buffer para cargar pixels(con espacio dinamico)
static uint8_t *s_pixel_buffer = NULL;

// Colores básicos (formato 8colores)
#define COLOR_BLACK    0b00000000//x0000
#define COLOR_WHITE    0b00111111//0xFFFF
#define COLOR_RED      0b00100100//0xF800
#define COLOR_GREEN    0b00010010//0x07E0
#define COLOR_BLUE     0b00001001//0x001F
#define COLOR_YELLOW   0b00110110//0xFFE0
#define COLOR_CYAN     0b00011011//0x07FF
#define COLOR_MAGENTA  0b00101101//0xF81F

// Funciones públicas
void display_init(void);
void display_clear(uint16_t color);
void display_draw_pixel(uint16_t x, uint16_t y, uint8_t color);
void display_draw_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void display_draw_character_buffered(uint16_t x, uint16_t y,uint16_t w,uint16_t h, uint8_t* buffer_char);
//void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
//void display_draw_string(const char *text, uint16_t x, uint16_t y, uint16_t color, font_size_t size);
//void display_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
//void display_start_frame(void);
//void display_end_frame(void);
//void display_set_brightness(uint8_t percent);//brillo








#endif // DISPLAY_H