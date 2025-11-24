idf_component_register(SRCS  "main.cpp" 
                    INCLUDE_DIRS "."
                    REQUIRES display gps LoRa adc TFT_eSPI arduino Decoder ESP32Servo PID_Servo)