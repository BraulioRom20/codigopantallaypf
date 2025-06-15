idf_component_register(
    SRCS "velocimetro.c" "fonts.c" "battery_ui.c" "adcfunction.c" "main.c" "display.c"
    INCLUDE_DIRS "."
    REQUIRES freertos esp_driver_gpio esp_driver_spi esp_adc
)
