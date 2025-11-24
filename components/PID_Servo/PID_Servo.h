#include "Arduino.h"
#include <ESP32Servo.h>

#define SERVO_PIN 27 //GPIO SERVO


//functions:

void servo_init();
void velocidad_crucero(float velocity);
void test_servo();