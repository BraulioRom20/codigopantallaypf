#include "display.h"
#include "spi.h"
#include "fonts.h"
#include <string.h>

void display_draw_text(const char* text, uint16_t x, uint16_t y, uint32_t color){
    // Dibuja el texto en la pantalla utilizando la fuente y el color especificados
    // Aquí se debe implementar la lógica para dibujar el texto en la pantalla
}
void display_clear(uint32_t color){
    // Limpia la pantalla con el color especificado
    // Aquí se debe implementar la lógica para limpiar la pantalla
}	
void display_init(void) {
    spi_init();
}

void display_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
}
