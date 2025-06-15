#pragma once
#include <stdint.h>

// Tipos de fuente disponibles
typedef enum {
    FONT_MEDIUM = 0,  
    FONT_LARGE        
} font_size_t;

// Colores RGB565 
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_GREEN     0x07E0
#define COLOR_RED       0xF800
#define COLOR_BLUE      0x001F

void display_draw_text(const char *text, uint16_t x, uint16_t y, uint16_t color, font_size_t size);
uint16_t get_text_width(const char *text, font_size_t size);