#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "adcfunction.h"
#include "battery_ui.h"
#include "velocimetro.h"
#include "display.h"

// Configuración del hardware
#define BUTTON_PIN         GPIO_NUM_0
#define DEBOUNCE_TIME_MS   50

// Variables globales
static QueueHandle_t button_events = NULL;
static CircularSpeedometer speedo;

// ISR para el pulsador (misma implementación que antes)
void IRAM_ATTR button_isr_handler(void* arg) {
    uint32_t pin = (uint32_t)arg;
    xQueueSendFromISR(button_events, &pin, NULL);
}

void app_main(void) {
    // 1. Inicialización general
    adc_init();
    display_init();
    
    // 2. Configuración directa del pulsador (sin input.c/h)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_conf);
    
    button_events = xQueueCreate(10, sizeof(uint32_t));
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, (void*)BUTTON_PIN);
    
    // 3. Inicialización de componentes
    battery_ui_init();
    speedometer_c_init(&speedo);
    
    // 4. Variables de estado
    uint8_t current_tab = 0; // 0=Velocímetro, 1=Batería
    float total_ah = 17.0f;
    float consumed_ah = 0.0f;
    uint32_t last_update = 0;
    float current_speed = 0.0f;

    while(1) {
        // --- Manejo del pulsador ---
        uint32_t pin_num;
        if(xQueueReceive(button_events, &pin_num, 0)) {
            vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_TIME_MS));    
            if(!gpio_get_level(pin_num)) {
                current_tab = !current_tab;
                display_clear(COLOR_BLACK); 
            }
        }
        
        // --- Lectura de sensores ---
        float voltage = read_voltage();
        float current = read_current();
        
        // Cálculo de capacidad
        uint32_t now = xTaskGetTickCount();
        if(last_update != 0) {
            float hours = (now - last_update) / (1000.0f * 60.0f * 60.0f);
            consumed_ah += current * hours;
        }
        last_update = now;
        
        float capacity = 100.0f * (1.0f - (consumed_ah / total_ah));
        capacity = (capacity < 0) ? 0 : capacity;
        
        // --- Lógica de velocidad (ejemplo) ---
        current_speed = get_current_speed(); // Implementar esta función
        
        // --- Renderizado ---
        display_start_frame();
        
        if(current_tab == 0) {
            speedometer_c_update(&speedo, (int)current_speed);
            speedometer_c_draw(&speedo);
        } else {
            battery_ui_update(voltage, current, capacity);
            battery_ui_draw();
        }
        
        // Indicador de pestaña (opcional)
        display_draw_string(10, 10, current_tab ? "BATERIA" : "VELOCIMETRO", 
                          FONT_MEDIUM, current_tab ? COLOR_GREEN : COLOR_RED);
        
        display_end_frame();
        
        vTaskDelay(pdMS_TO_TICKS(50)); // Control de FPS
    }
    
    // Limpieza (en caso necesario)
    speedometer_c_free(&speedo);
}