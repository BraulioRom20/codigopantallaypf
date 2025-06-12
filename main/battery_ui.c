#include "battery_ui.h"
#include "display.h"
#include "fonts.h"
#include <stdio.h>

static const uint16_t TEXT_COLOR = 0xFFFF;  // blanco
static const uint16_t LABEL_COLOR = 0x07E0; // verde
static const uint16_t BG_COLOR = 0x0000;    // negro

void battery_ui_init(void);
void battery_ui_draw_static_elements(void);
void battery_ui_update(float voltage, float current, float capacity);

void battery_ui_init(void) {
    display_clear(BG_COLOR);
    battery_ui_draw_static_elements();
}

void battery_ui_draw_static_elements(void) {
    display_draw_text("DESAFIO ECO YPF 2025", 50, 20, TEXT_COLOR);
    display_draw_text("Tension:", 30, 70, LABEL_COLOR);
    display_draw_text("Corriente:", 30, 120, LABEL_COLOR);
    display_draw_text("Capacidad:", 30, 170, LABEL_COLOR);
}

void battery_ui_update(float voltage, float current, float capacity) {
    char buffer[16];
    
    // Actualiza tensión
    snprintf(buffer, sizeof(buffer), "%.1f V", voltage);
    display_draw_text(buffer, 150, 70, TEXT_COLOR);
    
    // Actualiza corriente
    snprintf(buffer, sizeof(buffer), "%.1f A", current);
    display_draw_text(buffer, 150, 120, TEXT_COLOR);
    
    // Actualiza capacidad
    snprintf(buffer, sizeof(buffer), "%.0f %%", capacity);
    display_draw_text(buffer, 150, 170, TEXT_COLOR);
}