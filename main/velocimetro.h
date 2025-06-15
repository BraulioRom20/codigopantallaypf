#ifndef UI_H
#define UI_H

#include <stdint.h>
#include <math.h>

#define HALF_SCREEN_WIDTH  240
#define GAUGE_RADIUS       100  // Radio del velocímetro circular
#define GAUGE_CENTER_X     (HALF_SCREEN_WIDTH/2)
#define GAUGE_CENTER_Y     (GAUGE_RADIUS + 20)

typedef struct {
    uint16_t* buffer;
    int16_t width;
    int16_t height;
    uint16_t dial_color;
    uint16_t bg_color;
    uint16_t scale_color;
    uint16_t text_color;
    float last_angle; // Para animaciones suaves
} CircularSpeedometer;

void ui_init(void);
void speedometer_c_init(CircularSpeedometer* sp);
void speedometer_c_update(CircularSpeedometer* sp, int speed);
void speedometer_c_draw(CircularSpeedometer* sp);

// Helper function to convert integer to string for display
void int_to_str(int value, char* buffer, int buffer_size);
void speedometer_c_free(CircularSpeedometer* sp);

#endif