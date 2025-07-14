#include <stdio.h>
#include <display.h>
#include "lora.h"
#include "neo6m.h"
#include "adcfunction.h"
#include "Arduino.h"
#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_GREY 0x5AEB // New colour


extern "C" void app_main()
{
  tft.init();
  tft.setRotation(2);

  while(1){
    // fondo de color plata (ver como queda con maroon también)
  tft.fillScreen(TFT_SILVER);
  
  // pongo el cursor en la cordenada (10,0) de la pantalla y elijo la fuente 4 
  // salta a la siguiente linea si es "tft.println" o se queda en la misma si es "tft.print"
  tft.setCursor(30, 0, 4);
  // tft.setTextColor(TFT_NAVY, TFT_SILVER); // primero color de texto, segundo color de fondo
  tft.setTextColor(TFT_NAVY);  tft.setTextSize(2); 
  // ahora podes escribir con tft.print() o tft.println()
  tft.println("DESAFIO ECO YPF");

  tft.setCursor(40, 0, 4);
  tft.setTextColor(TFT_RED); 
  tft.println("PARAMETROS");
  
  // pongo el cursor en la cordenada (0,60) de la pantalla y elijo la fuente 4
  tft.setCursor(0, 60, 4);
  // azul con la misma tipografia, pero mas chico
  tft.setTextColor(TFT_NAVY);
  tft.setTextFont(4);
  tft.setTextSize(1);

  float tension = 48.0; // Luego modificar esta por la variable que almacena la tension de las baterias
  float tensionMax = 48.0; // Variable que almacena la tension maxima de las baterias
  // Imprimir la tension de las baterias
  tft.print("Tension de las Baterias = "); tft.println(tension); tft.println();

  float corriente = 17.0; // Luego modificar esta por la variable que almacena la corriente de las baterias
  float corrienteMax = 20.0; // Variable que almacena la corriente maxima de las baterias
  // Imprimir la corriente de las baterias
  tft.print("Corriente de las Baterias = "); tft.println(corriente); tft.println();

  float capacidad = 99.9; // Luego modificar esta por la variable que almacena la capacidad  de las baterias
  float capacidadMax = 100.0; // Variable que almacena la capacidad maxima de las baterias
  // Imprimir la capacidad de las baterias
  tft.print("Capacidad de las Baterias = "); tft.println(capacidad); tft.println();


  bool velocidadCruceroActiva = true; // o false

  tft.print("Velocidad Crucero = ");
  if (velocidadCruceroActiva) {
    tft.println("ON");
  } else {
    tft.println("OFF");
  }


  bool launchControl = true; // o false

  tft.print("Launch Control = ");
  if (launchControl) {
    tft.println("ON");
  } else {
    tft.println("OFF");
  }


  bool estadoDelGps = true; // o false

  tft.print("Estado del GPS = ");
  if (estadoDelGps) {
    tft.println("ON");
  } else {
    tft.println("OFF");
  }
  
  //se pueden guardar strings en FLASH para ahorrar RAM
  //tft.println(F("my foonting turlingdromes.")); 
  // si la linea es muy larga, se corta y se salta a la siguiente linea
  //tft.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");
  //tft.print("Binary = "); tft.println((int)tension, BIN); // Print as integer value in binary
  //tft.print("Hexadecimal = "); tft.println((int)tension, HEX); // Print as integer number in Hexadecimal
  delay(10000);
  }
}

/*
// 3. Inicialización MPU6050
    ESP_LOGI(TAG, "Inicializando MPU6050...");
    if(mpu6050_init(I2C_NUM_0) != ESP_OK) {
        ESP_LOGE(TAG, "Error inicializando MPU6050");
        return;
    }

    // 4. Bucle principal
    mpu6050_data_t data;
    while(1) {
        if(mpu6050_read(&data, I2C_NUM_0) == ESP_OK) {
            ESP_LOGI(TAG, "Acelerómetro: X=%d Y=%d Z=%d | Giroscopio: X=%d Y=%d Z=%d",
                    data.ax, data.ay, data.az,
                    data.gx, data.gy, data.gz);
        }
        vTaskDelay(pdMS_TO_TICKS(500)); // 500ms entre lecturas
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

