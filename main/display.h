#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// Tipos de fuente
typedef enum {
    FONT_SMALL,
    FONT_MEDIUM,
    FONT_LARGE,
    FONT_XLARGE
} font_size_t;

// Tamaño del display
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

// Colores básicos (formato RGB565)
#define COLOR_BLACK    0x0000
#define COLOR_WHITE    0xFFFF
#define COLOR_RED      0xF800
#define COLOR_GREEN    0x07E0
#define COLOR_BLUE     0x001F
#define COLOR_YELLOW   0xFFE0
#define COLOR_CYAN     0x07FF
#define COLOR_MAGENTA  0xF81F

// Funciones públicas
void display_init(void);
void display_clear(uint16_t color);
void display_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void display_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void display_draw_string(const char *text, uint16_t x, uint16_t y, uint16_t color, font_size_t size);
void display_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap);
void display_start_frame(void);
void display_end_frame(void);
void display_set_brightness(uint8_t percent);

#endif // DISPLAY_H