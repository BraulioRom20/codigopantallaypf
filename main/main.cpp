#include <stdio.h>
#include "neo6m.h"
#include "adcfunction.h"
#include "Arduino.h"
#include "windows.h"
#include <SPI.h>
#include <src/LoRa.h>
#include "definitions.h"
#include "ESP32Servo.h"
#include "PID_Servo.h"

//Parameters vars:

float tension; float corriente; float capacidad;  bool launchControl; bool estadoDelGps;

//GPS vars:

double latitude; double longitude; char lat_hemisphere; char lon_hemisphere; float velocidad;

//DISPLAY VAR

uint8_t windowMode = 0;

//auto-pilot var

bool AutoPilot = false;

/*

  BUTTONS INTERRUPTS:

*/

// Interrupt Service Routines (ISRs)
void IRAM_ATTR handleButton1() {
  button1Pressed = true;
  //Serial.println("hola");
}

void IRAM_ATTR handleButton2() {
  button2Pressed = true;
}

void IRAM_ATTR handleButton3() {
  button3Pressed = true;
}

void init_interruptions() {

  // Configure button pins as input with pull-up resistor
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLDOWN);
  pinMode(buttonPin3, INPUT_PULLDOWN);

  pinMode(2, OUTPUT);


  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(buttonPin1), handleButton1, FALLING);  // FALLING if using INPUT_PULLUP
  attachInterrupt(digitalPinToInterrupt(buttonPin2), handleButton2, RISING);
  //attachInterrupt(digitalPinToInterrupt(buttonPin3), handleButton3, RISING);

  //BOCINA

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUZZER_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&io_conf);
}

///////////////////////////////////////////////////////////////////////////////////

/*
  PARALLEL TASK


  //Read BAT, GPS and send it .


*/


void telemetryRead(void  *pvParameters){
    
    raw_nmea(&latitude,&longitude,&lat_hemisphere,&lon_hemisphere,&velocidad);
    //-------read voltage and current and batterys percent


    //DEBUG
    //ESP_LOGI(TAG,"lat: %lf / %c ^^ lon; %lf / %c",latitude,lat_hemisphere,longitude,lon_hemisphere);  
    //ESP_LOGI(TAG,"velocity: %lf",speedKmh);
    
    //SEND DATA

    /*
    LoRa.beginPacket();
    LoRa.print("hello");
    LoRa.endPacket();
    */
    
}














//////////////////////////////////////////////////////////////////////////////////////
/*

  *SETUP*

*/


extern "C" void app_main()  {
  delay(1000);//DESP. SACAR

  //init serial for debuging
  Serial.begin(115200);
  Serial.println("SERIAL OK");

  init_interruptions();
  Serial.println("INTERRUPTIONS OK");

  //INIT DISPLAY

  //***************** TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
  
  //GPS
  gps_starting();//avoid using gps_starting() again until and unless driver is not de-inited properly.
  ESP_LOGI("GPS", "OK");


	//LORA TX:

  /*
    ****************************************************
    pinMode(5,OUTPUT);
  SPI.begin(LORA_SCK,LORA_MISO,LORA_MOSI,LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections and frequency!");
    while (true); // Halt execution
  }else{Serial.println("LoRa OK");}

  */
  
  
  //SERVO
  servo_init(); 
  Serial.println("SERVO OK");

  /*
    PARALLEL TASK ATTACH


    xTaskCreatePinnedToCore(
        telemetryRead,     
        "telemetry",        
        2048,                  // Stack size
        NULL,                  // Parameter
        5,                     // Priority
        NULL,                  // Task handle
        1                      // Core 0 or 1
    );

  */
    
    




  
 
  /*
    MAIN TASK
  */

  while(1){
    /*

    Window change:


    if(windowMode == 0){
      print_page_all_parameters( tension,  corriente,  capacidad,  velocidad,   velocidadCruceroActiva,   launchControl,  estadoDelGps);
    }else{print_tachometer();}
    */
    
    /*

    BUTTON 1 BUTTON 2

      if (!button1Pressed) {
      button1Pressed = false;
      Serial.println("Button 1 was pressed!");
      //active horn
      analogWrite(BUZZER_PIN,128);
    }else{
      analogWrite(BUZZER_PIN,0);
    }

    if (!button2Pressed) {
      button2Pressed = false;
      Serial.println("Button 2 was pressed!");
      //change window
      windowMode = 1 - windowMode;
    }


    */
    
    
    //raw_nmea(&latitude,&longitude,&lat_hemisphere,&lon_hemisphere,&velocidad);  

    /*
      if (!button3Pressed) {
      button3Pressed = false;
      Serial.println("Button 3 was pressed!(autopilot state changed)");
      //active or desactive automatic pilot
      AutoPilot = !AutoPilot;
    }
    */
    test_servo();
    
    if(digitalRead(buttonPin3)){
      //button3Pressed = false;
      Serial.println("Button 3 was pressed!(autopilot state changed)");
      //active or desactive automatic pilot
      AutoPilot = !AutoPilot;
      
    }

    if(AutoPilot == true){
      velocidad_crucero(velocidad);
      digitalWrite(2,HIGH);
    }else{
      digitalWrite(2,LOW);
    }

  delay(1000);
  }
}  
  













/*
while(1){
    uint16_t angle = random(241); // random speed in range 0 to 240

    // Plot needle at random angle in range 0 to 240, speed 40ms per -increment
    plotNeedle(angle, 0);
  
    // Pause at new position
    delay(2500);
  
}
    */
  
















/*  
 Test the tft.print() viz. the libraries embedded write() function

 This sketch used font 2, 4, 7
 
 Make sure all the required fonts are loaded by editing the
 User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
 */