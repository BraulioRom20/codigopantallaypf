#include "battery_ui.h"
#include "display.h"
#include "fonts.h"
#include <stdio.h>

static const uint16_t TEXT_COLOR = 0xFFFF;  // Blanco
static const uint16_t LABEL_COLOR = 0x07E0; // Verde
static const uint16_t BG_COLOR = 0x0000;    // Negro

void battery_ui_init(void) {
    display_clear(BG_COLOR);
    battery_ui_draw_static_elements();
}

void battery_ui_draw_static_elements(void) {
    display_draw_text("DESAFIO ECO YPF 2025", 50, 20, TEXT_COLOR, FONT_LARGE);
    display_draw_text("Tension:", 30, 70, LABEL_COLOR, FONT_MEDIUM);
    display_draw_text("Corriente:", 30, 120, LABEL_COLOR, FONT_MEDIUM);
    display_draw_text("Capacidad:", 30, 170, LABEL_COLOR, FONT_MEDIUM);
}

void battery_ui_update(float voltage, float current, float capacity) {
    char buffer[16];
    
    // Actualiza tensión
    snprintf(buffer, sizeof(buffer), "%.1f V", voltage);
    display_draw_text(buffer, 150, 70, TEXT_COLOR, FONT_MEDIUM);
    
    // Actualiza corriente
    snprintf(buffer, sizeof(buffer), "%.1f A", current);
    display_draw_text(buffer, 150, 120, TEXT_COLOR, FONT_MEDIUM);
    
    // Actualiza capacidad
    snprintf(buffer, sizeof(buffer), "%.0f %%", capacity);
    display_draw_text(buffer, 150, 170, TEXT_COLOR, FONT_MEDIUM);
}