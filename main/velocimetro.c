#include "velocimetro.h"
#include "display.h"
#include "fonts.h"

// Función para dibujar círculo
static void draw_circle(uint16_t* buffer, int width, int cx, int cy, int radius, uint16_t color) {
    for(int y = -radius; y <= radius; y++) {
        for(int x = -radius; x <= radius; x++) {
            if(x*x + y*y <= radius*radius) {
                int px = cx + x;
                int py = cy + y;
                if(px >= 0 && px < width && py >= 0 && py < GAUGE_RADIUS*2 + 40) {
                    buffer[px + py * width] = color;
                }
            }
        }
    }
}

// Función para dibujar línea (dial)
static void draw_line(uint16_t* buffer, int width, int x1, int y1, int x2, int y2, uint16_t color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;
    
    while(1) {
        buffer[x1 + y1 * width] = color;
        if(x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if(e2 >= dy) { err += dy; x1 += sx; }
        if(e2 <= dx) { err += dx; y1 += sy; }
    }
}

void speedometer_c_init(CircularSpeedometer* sp) {
    sp->width = HALF_SCREEN_WIDTH;
    sp->height = GAUGE_RADIUS*2 + 40;
    sp->buffer = (uint16_t*)malloc(sp->width * sp->height * sizeof(uint16_t));
    
    // Colores
    sp->dial_color = COLOR_RED;
    sp->bg_color = COLOR_BLACK;
    sp->scale_color = COLOR_WHITE;
    sp->text_color = COLOR_BLACK;
    sp->last_angle = -M_PI/2; // Inicia en posición "0 km/h"
}

void speedometer_c_update(CircularSpeedometer* sp, int speed) {
    // 1. Limpiar buffer
    for (int i = 0; i < sp->width * sp->height; ++i) {
        sp->buffer[i] = sp->bg_color;
    }
    
    // 2. Dibujar escala circular
    draw_circle(sp->buffer, sp->width, GAUGE_CENTER_X, GAUGE_CENTER_Y, GAUGE_RADIUS, sp->scale_color);
    
    // 3. Dibujar marcas cada 20 km/h
    for(int kmh = 0; kmh <= 200; kmh += 20) {
        float angle = -M_PI/2 + (kmh / 200.0f) * M_PI; // Rango de 90° a -90°
        int x1 = GAUGE_CENTER_X + (GAUGE_RADIUS-10) * cos(angle);
        int y1 = GAUGE_CENTER_Y + (GAUGE_RADIUS-10) * sin(angle);
        int x2 = GAUGE_CENTER_X + GAUGE_RADIUS * cos(angle);
        int y2 = GAUGE_CENTER_Y + GAUGE_RADIUS * sin(angle);
        
        draw_line(sp->buffer, sp->width, x1, y1, x2, y2, sp->scale_color);
        
        // Texto de marca
        char mark[4];
        snprintf(mark, sizeof(mark), "%d", kmh);
        int tx = GAUGE_CENTER_X + (GAUGE_RADIUS-25) * cos(angle) - 10;
        int ty = GAUGE_CENTER_Y + (GAUGE_RADIUS-25) * sin(angle);
        if (tx >= 0 && tx < sp->width && ty >= 0 && ty < sp->height) {
            display_draw_string(tx, ty, mark, sp->text_color, FONT_LARGE);
        }
    }
    
    // 4. Calcular ángulo del dial (suavizado para animación)
    float target_angle = -M_PI/2 + (speed / 200.0f) * M_PI;
    sp->last_angle += (target_angle - sp->last_angle) * 0.2; 
    // 5. Dibujar dial
    int dial_x = GAUGE_CENTER_X + (GAUGE_RADIUS-20) * cos(sp->last_angle);
    int dial_y = GAUGE_CENTER_Y + (GAUGE_RADIUS-20) * sin(sp->last_angle);
    draw_line(sp->buffer, sp->width, GAUGE_CENTER_X, GAUGE_CENTER_Y, dial_x, dial_y, sp->dial_color);
    
    // 6. Dibujar valor numérico central
    char speed_str[10];
    snprintf(speed_str, sizeof(speed_str), "%d", speed);
    display_draw_string(GAUGE_CENTER_X-30, GAUGE_CENTER_Y-10, speed_str, sp->text_color, FONT_LARGE);
    
    // 7. Dibujar unidad
    display_draw_string(GAUGE_CENTER_X-20, GAUGE_CENTER_Y+20, "km/h", sp->text_color, FONT_MEDIUM);
}

void speedometer_c_draw(CircularSpeedometer* sp) {
    // If display_draw_buffer is not available, draw pixel by pixel
    for (int y = 0; y < sp->height; ++y) {
        for (int x = 0; x < sp->width; ++x) {
            display_draw_pixel(x, y, sp->buffer[x + y * sp->width]);
        }
    }
}

void speedometer_c_free(CircularSpeedometer* sp) {
    free(sp->buffer);
    sp->buffer = NULL;
}