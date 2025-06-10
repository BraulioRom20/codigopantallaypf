
#pragma once
#include <stdint.h>
#include "driver/spi_master.h"

void display_init(void);
void display_draw_text(const char* text, uint16_t x, uint16_t y, uint32_t color);
void display_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
void display_clear(uint32_t color);