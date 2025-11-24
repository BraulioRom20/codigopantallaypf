#include "windows.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft); // Sprite object for needle
TFT_eSprite spr    = TFT_eSprite(&tft); // Sprite for meter reading

extern uint16_t angle;

uint16_t* tft_buffer;
//bool      buffer_loaded = false;
uint16_t  spr_width = 0;
uint16_t  bg_color =0;

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

void print_page_all_parameters(float tension, float corriente, float capacidad, int velocidad,  bool velocidadCruceroActiva,  bool launchControl, bool estadoDelGps)
{


  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  //tft.init();
  //tft.setRotation(2);

    // fondo de color plata (ver como queda con maroon también)
  //tft.fillScreen(TFT_BLACK);
  
  // pongo el cursor en la cordenada (10,0) de la pantalla y elijo la fuente 4 
  // salta a la siguiente linea si es "tft.println" o se queda en la misma si es "tft.print"
  tft.setCursor(50, 10, 4);
  // tft.setTextColor(TFT_NAVY, TFT_SILVER); // primero color de texto, segundo color de fondo
  tft.setTextColor(TFT_GOLD);  tft.setTextSize(1); 
  // ahora podes escribir con tft.print() o tft.println()
  tft.println("DESAFIO ECO YPF");

  tft.setCursor(70, 40, 4);
  tft.println("PARAMETROS");
  
  // pongo el cursor en la cordenada (10,80) de la pantalla y elijo la fuente 4
  tft.setCursor(10, 90, 4);
  // azul con la misma tipografia, pero mas chico
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(4);
  tft.setTextSize(0.5);

  
  float tensionMax = 48.0; // Variable que almacena la tension maxima de las baterias
  // Imprimir la tension de las baterias
  tft.print("Tension = "); 
  tft.setTextColor(TFT_GOLD); tft.print(tension); tft.print("V / "); tft.print(tensionMax); tft.println("V");

  tft.setCursor(10, 125, 4);
  
  float corrienteMax = 20.0; // Variable que almacena la corriente maxima de las baterias
  // Imprimir la corriente de las baterias
  tft.setTextColor(TFT_WHITE); tft.print("Corriente = "); 
  tft.setTextColor(TFT_GOLD); tft.print(corriente); tft.print("A / "); tft.print(corrienteMax); tft.println("A");
  
  tft.setCursor(10, 160, 4);
  
  // Imprimir la capacidad de las baterias
  tft.setTextColor(TFT_WHITE); tft.print("Capacidad = "); 
  tft.setTextColor(TFT_GOLD); tft.print(capacidad); tft.println("% / 100%");

  tft.setCursor(10, 230, 4);

  // Imprimir la capacidad de las baterias
  tft.setTextColor(TFT_WHITE); tft.print("Velocidad = "); 
  tft.setTextColor(TFT_GOLD); tft.print(velocidad); tft.println("km/h");

  tft.setCursor(10, 300, 4);

  tft.setTextColor(TFT_WHITE); tft.print("Velocidad Crucero = ");
  tft.setTextColor(TFT_GOLD);
  if (velocidadCruceroActiva) {
    tft.println("ON"); 
  } else {
    tft.println("OFF");
  }
  
  tft.setCursor(10, 335, 4);

  tft.setTextColor(TFT_WHITE); tft.print("Launch Control = ");
  tft.setTextColor(TFT_GOLD);
  if (launchControl) {
    tft.println("ON");
  } else {
    tft.println("OFF");
  }
  tft.println();




  tft.setCursor(10, 370, 4);

  tft.setTextColor(TFT_WHITE); tft.print("Estado del GPS = ");
  tft.setTextColor(TFT_GOLD);
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
}


void print_tachometer()
{
    TJpgDec.setSwapBytes(true);

  // The jpeg decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  

  // Draw the dial
  TJpgDec.drawJpg(40, 120, dial, sizeof(dial));
  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, NEEDLE_RADIUS-NEEDLE_LENGTH, TFT_DARKGREY);

  // Load the font and create the Sprite for reporting the value
  spr.loadFont(AA_FONT_LARGE);
  spr_width = spr.textWidth("777"); // 7 is widest numeral in this font
  spr.createSprite(spr_width, spr.fontHeight());
  bg_color = TFT_BLACK;
  //bg_color = tft.readPixel(160, 240); // Get colour from dial centre
  spr.fillSprite(bg_color);
  spr.setTextColor(TFT_WHITE, bg_color, true);
  spr.setTextDatum(MC_DATUM);
  spr.setTextPadding(spr_width);
  spr.drawNumber(0, spr_width/2, spr.fontHeight()/2);
  spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);

  // Plot the label text
  tft.setTextColor(TFT_WHITE, bg_color);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(" km/h ", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

  // Define where the needle pivot point is on the TFT before
  // creating the needle so boundary calculation is correct
  tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);
}

void update_tachometer(int16_t velocidad, uint16_t ms_delay)
{
  // Bounding box parameters
  static int16_t min_x = 40;
  static int16_t min_y = 120;
  static int16_t max_x = 240;
  static int16_t max_y = 320;

  if (angle < 0) angle = 0; // Limit angle to emulate needle end stops
  if (angle > 240) angle = 240;

  spr.setTextColor(TFT_WHITE, bg_color, true);
  spr.drawNumber(angle, spr_width/2, spr.fontHeight()/2);
  spr.pushSprite(160 - spr_width / 2, 240 - spr.fontHeight() / 2);
  
}















// =======================================================================================
// Create the needle Sprite
// =======================================================================================
void createNeedle(void)
{
  needle.setColorDepth(16);
  needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite
  
  needle.fillSprite(TFT_BLACK); // Fill with black
  

  // Define needle pivot point relative to top left corner of Sprite
  uint16_t piv_x = NEEDLE_WIDTH / 2; // pivot x in Sprite (middle)
  uint16_t piv_y = NEEDLE_RADIUS;    // pivot y in Sprite
  needle.setPivot(piv_x, piv_y);     // Set pivot point in this Sprite


  // Draw the red needle in the Sprite
  needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
  needle.fillRect(1, 1, NEEDLE_WIDTH-2, NEEDLE_LENGTH-2, TFT_RED);

  // Bounding box parameters to be populated
  int16_t min_x = 40;
  int16_t min_y = 120;
  int16_t max_x = 240;
  int16_t max_y = 320;

  // Work out the worst case area that must be grabbed from the TFT,
  // this is at a 45 degree rotation
  needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

  // Calculate the size and allocate the buffer for the grabbed TFT area
  tft_buffer =  (uint16_t*) malloc( ((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2 );
  //uint16_t *tft_buffer = (uint16_t *)heap_caps_malloc(( ((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2 ), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  

}