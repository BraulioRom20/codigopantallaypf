#pragma once

typedef enum {
    SCREEN_SPEED,
    SCREEN_BATTERY
} screen_t;

void ui_init(void);
void ui_update(void);