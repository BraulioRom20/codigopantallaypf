#pragma once
#include <stdint.h>

void battery_ui_init(void);
void battery_ui_update(float voltage, float current, float capacity);
void battery_ui_draw_static_elements(void);