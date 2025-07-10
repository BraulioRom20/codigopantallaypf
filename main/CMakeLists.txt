idf_component_register(SRCS  "main.cpp" 
                    INCLUDE_DIRS "."
                    REQUIRES mpu6050 display gps lora adc TFT_eSPI arduino)