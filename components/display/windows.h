#include <SPI.h>
#include "NotoSansBold36.h"
#include "dial.h"
#include <src/TJpg_Decoder.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include "Arduino.h"
#include "stdio.h"

#define TFT_GREY 0x5AEB // New colour

#define NEEDLE_LENGTH 35  // Visible length
#define NEEDLE_WIDTH   5  // Width of needle - make it an odd number
#define NEEDLE_RADIUS 90  // Radius at tip
#define NEEDLE_COLOR1 TFT_MAROON  // Needle periphery colour
#define NEEDLE_COLOR2 TFT_RED     // Needle centre colour
#define DIAL_CENTRE_X 160
#define DIAL_CENTRE_Y 240

// Font attached to this sketch

#define AA_FONT_LARGE NotoSansBold36



void print_page_all_parameters(float tension, float corriente, float capacidad, int velocidad,  bool velocidadCruceroActiva,  bool launchControl, bool estadoDelGps);
void print_tachometer();
void update_tachometer(uint16_t velocidad, uint16_t ms_delay);