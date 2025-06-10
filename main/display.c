#include "display.h"
#include "spi.h"
#include "fonts.h"
#include <string.h>

static const uint16_t DISPLAY_WIDTH = 320;
static const uint16_t DISPLAY_HEIGHT = 480;

void display_init(void) {
    spi_init();
}

void display_draw_text(const char* text, uint16_t x, uint16_t y, uint16_t color, font_size_t font) {
}