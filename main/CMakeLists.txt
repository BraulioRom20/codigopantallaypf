idf_component_register(
    SRCS "spi.c" "battery_ui.c" "adc.c" "main.c" "display.c" "ui.c" "input.c"
    INCLUDE_DIRS "."
    REQUIRES freertos esp_driver_gpio esp_driver_spi
)