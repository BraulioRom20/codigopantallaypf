#include "PID_Servo.h"

Servo motor;

// --- PID parameters ---
// constantes calculadas a través del método Tyreus-Luyben
double Kp = 3.6;
double Ki = 0.1;
double Kd = 1.76;

// --- Control variables ---
double setpoint = 20.0;   // Desired velocity (km/h) - velocidad seteada para modo crucero

double input = 0.0;       // Measured velocity (km/h)
double output = 0.0;      // Output to servo (µs)

double lastError = 0.0;
double integral = 0.0;

// --- Timing ---
unsigned long lastTime = 0;
const unsigned long sampleTime = 100; // ms 100ms default DESPUES HAY QUE BAJARLE

void servo_init(){
    motor.setPeriodHertz(50);
    motor.attach(SERVO_PIN,1000,2000);
}



void velocidad_crucero(float velocity) {
  unsigned long now = millis();
  double deltaTime = (now - lastTime) / 1000.0; // convert to seconds

  if (now - lastTime >= sampleTime) {
    lastTime = now;

    // --- Read sensor value (velocity in km/h) ---
    input = velocity;

    // --- Compute PID manually ---
    double error = setpoint - input;
    integral += error * deltaTime;
    double derivative = (error - lastError) / deltaTime;

    output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // --- Limit the output to servo range (1000–2000 µs) ---
    if (output > 2000) output = 2000;
    else if (output < 1000) output = 1000;

    // --- Apply output to servo ---
    motor.writeMicroseconds((int)output);

    // --- Save last error ---
    lastError = error;

    // --- Debug info ---
    Serial.print("Velocity: ");
    Serial.print(input);
    Serial.print(" km/h | Setpoint: ");
    Serial.print(setpoint);
    Serial.print(" | Output: ");
    Serial.println(output);
  }}

void test_servo(){
  //motor.writeMicroseconds(2000); //1000 o 2000

  //motor.writeMicroseconds(1000);
  motor.write(30);
  delay(3000);
  //motor.writeMicroseconds(2000);
  motor.write(90);
  delay(1000);

}
