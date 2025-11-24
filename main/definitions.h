// 
// LoRa pins for ESP32 TX
//

#define LORA_SCK 18
#define LORA_MISO 22 // **CHANGE**
#define LORA_MOSI 23
#define LORA_SS 5

#define LORA_RST 3 //not use
#define LORA_DIO0 21 //not use

/*
    INTERRUPTS
*/

#define buttonPin1 39  //option 1 -- change windown
#define buttonPin2 34  //option 2 -- automatic pilot
#define buttonPin3 35  //option 3 -- launch control

// Volatile variables used in ISR
volatile bool button1Pressed;
volatile bool button2Pressed;
volatile bool button3Pressed;

/*
    BOCINA
*/

#define BUZZER_PIN 14