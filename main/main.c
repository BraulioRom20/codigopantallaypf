#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "adc.h"
#include "battery_ui.h"
#include "display.h"

void app_main(void) {
    // Inicialización
    adc_init();
    display_init();
    battery_ui_init();

    // esquizofrenia (calculos)
    float total_ah = 17.0f;  // capacidad total teoricamente
    float consumed_ah = 0.0f;
    uint32_t last_update = 0;

    while(1) {
        // sensar dios sabra que hice aca
        float voltage = read_voltage();
        float current = read_current();
        
        // calculo raro de capacidad que vi en no se donde
        uint32_t now = xTaskGetTickCount();
        if(last_update != 0) {
            float hours = (now - last_update) / (1000.0f * 60.0f * 60.0f);
            consumed_ah += current * hours;
        }
        last_update = now;
        
        float capacity = 100.0f * (1.0f - (consumed_ah / total_ah));
        capacity = (capacity < 0) ? 0 : capacity;

        // no se, esto actualiza algo
        battery_ui_update(voltage, current, capacity);
        
        vTaskDelay(pdMS_TO_TICKS(1000));  // delay pq explota todo sino
    }
}