#include "display.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "string.h"
#include <stddef.h>
#include "fonts.h"



spi_device_handle_t spi;
static uint16_t* frame_buffer = NULL;


// comandos
static void send_cmd(uint8_t cmd) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = 8,
        .tx_buffer = &cmd,
        .user = (void*)0
    };
    
    gpio_set_level(PIN_SPI_DC, 0);
    ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);
}

// datos
static void send_data(uint8_t *data, uint16_t len) {
    esp_err_t ret;
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data,
        .user = (void*)1
    };
    
    gpio_set_level(PIN_SPI_DC, 1);
    ret = spi_device_polling_transmit(spi, &t);
    assert(ret == ESP_OK);
}

// this funcion sets the area to draw.
static void set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    uint8_t data[4];

    // Column Address Set (CASET)
    
    send_cmd(CMD_CASET);
    data[0] = (x0 >> 8) & 0xFF; data[1] = x0 & 0xFF; //high byte & low byte
    data[2] = (x1 >> 8) & 0xFF; data[3] = x1 & 0xFF; //high byte & low byte
    send_data(data, 4);
    
    // Page Address Set (RASET)

    send_cmd(CMD_RASET);
    data[0] = (y0 >> 8) & 0xFF; data[1] = y0 & 0xFF; //high byte & low byte
    data[2] = (y1 >> 8) & 0xFF; data[3] = y1 & 0xFF; //high byte & low byte
    send_data(data, 4);

    // Memory Write (RAMWR)

    send_cmd(CMD_RAMWR);// termino de establecer solo la escritura de pixels en esta area determinada
}







void display_init(void) {
    // SPI
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,
        .mosi_io_num = PIN_SPI_MOSI,
        .sclk_io_num = PIN_SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * 2,
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 26 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_SPI_CS,
        .queue_size = 7,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    // SPI
    ESP_ERROR_CHECK(spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(HSPI_HOST, &devcfg, &spi));

    // pines
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_SPI_DC) | (1ULL << PIN_SPI_RST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    // reset
    gpio_set_level(PIN_SPI_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(PIN_SPI_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    
    // secuencia para iniciar
    send_cmd(CMD_SWRESET);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Common bits: MX (flip X), MY (flip Y), MV (Row/Column Exchange - portrait/landscape)
    // For portrait: 0x48 (MY | BGR if using BGR panel)
    // For landscape: 0x28 (MX | MV | BGR)
    send_cmd(0x36); // MADCTL
    //establezco la forma de administrar el espacio del display por default
    send_data(NORMAL_MODE,1); // Example: Normal (no flip), RGB order. Adjust for your display/orientation!
    vTaskDelay(pdMS_TO_TICKS(120));
    // Example: Pixel Format Set (RGB565 = 0x55)

    send_cmd(0x3A); // COLMOD pixel format 
    send_data(PIXEL_FORMAT_3BIT,1); // 16-bit / pixel (RGB565)

    vTaskDelay(pdMS_TO_TICKS(120));


    send_cmd(CMD_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(120));
    send_cmd(CMD_DISPON);
    vTaskDelay(pdMS_TO_TICKS(120));

    vTaskDelay(pdMS_TO_TICKS(2000));
    // framebuffer (esto explota porque es demasiado la memoria para almazenar)
    //frame_buffer = (uint16_t*)heap_caps_malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA);
    //assert(frame_buffer != NULL);//here crashes

    ESP_LOGI(TAG, "Display inicializado. Tamaño: %dx%d", DISPLAY_WIDTH, DISPLAY_HEIGHT);

    //display_draw_area(0,0,320,480,COLOR_BLUE);
    ESP_LOGI(TAG, "Display impreso");

    char* text_to_display = "1234";
    
    display_draw_text_buffered(text_to_display,50,50,COLOR_BLUE, COLOR_WHITE, FONT_MEDIUM);
    //display_draw_area(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,COLOR_GREEN);
    //set_window(100, 100, 100 + 16-1, 100 + 12-1);
    //char_test = (uint8_t *)heap_caps_malloc(DMA_BUFFER_SIZE_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    //size_t buffer_size = (size_t)16 * 12 * 1; // 1 byte per pixel
    
    
    //uint8_t *pixel_buffer = (uint8_t *)heap_caps_malloc(DMA_BUFFER_SIZE_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);//(uint8_t*)malloc(buffer_size);
    //pixel_buffer = char_test;

   
}






//funciona
void display_draw_area(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t color) {
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
    if ((x + w) > DISPLAY_WIDTH) w = DISPLAY_WIDTH - x;
    if ((y + h) > DISPLAY_HEIGHT) h = DISPLAY_HEIGHT - y;

    set_window(x, y, x + w - 1, y + h - 1);

    uint32_t num_pixels = w * h;
    
    //control de cuantas pixeles me quedan por enviar
    uint32_t pixels_remaining = num_pixels;
    //cantidad de pixel a enviar
    uint32_t pixels_to_send;
    // lleno el buffer con cada DMA_BUFFER_SIZE_PIXELS Y envio.
    s_pixel_buffer = (uint8_t *)heap_caps_malloc(DMA_BUFFER_SIZE_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    while(pixels_remaining > 0){
    pixels_to_send = (pixels_remaining > DMA_BUFFER_SIZE_PIXELS) ? DMA_BUFFER_SIZE_PIXELS : pixels_remaining;
    //ESP_LOGI(TAG,"pixels init buffer");
    //vTaskDelay(pdMS_TO_TICKS(1000));
    for (int i = 0; i < pixels_to_send; i++) {
            s_pixel_buffer[i] = color; // here crashes
            //ESP_LOGI(TAG,"pixels to send %d",i);
        }
        
    send_data(s_pixel_buffer,pixels_to_send);
    pixels_remaining -= pixels_to_send;
    }
}


void display_draw_character_buffered(uint16_t x, uint16_t y,uint16_t w,uint16_t h, uint8_t* buffer_char){
    set_window(x, y, x + w-1, y + h-1);// seteo area a dibujar caracter
    uint8_t cont = 0;
    for(uint16_t i = 0; i<h*w;i++){
            //ESP_LOGI(TAG,"N:%u Dato: %u",i,buffer_char[i]);
            if(buffer_char[i] == 63){printf("0 ");}else{printf("1 ");}
            
            //send_data(&buffer_char[i],1);// envio info de caracter en forma de buffer
            vTaskDelay(pdMS_TO_TICKS(1));cont++;
            if(cont == 16){
                printf("\n");cont = 0;
            }
            //12*16 the dimensions in bytes of the font medium.
        }
    
    send_data(buffer_char,w*h/* *2*(?) */);// envio info de caracter en forma de buffer

    
    
    ESP_LOGI(TAG,"character wrote");
    
}











/*
void display_draw_string(const char *text, uint16_t x, uint16_t y, uint16_t color, font_size_t size) {
    // implementación simple
    const uint8_t *font = get_font(size);
    uint16_t font_width = get_font_width(size);
    uint16_t font_height = get_font_height(size);
    
    while(*text) {
        draw_char(*text, x, y, color, font, font_width, font_height);
        x += font_width;
        text++;
    }
}
*/




















void display_draw_bitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
    set_window(x, y, x + w - 1, y + h - 1);
    send_cmd(CMD_RAMWR);
    send_data((uint8_t*)bitmap, w * h * 2);
}






//no implementado
void display_clear(uint16_t color) {
    for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        frame_buffer[i] = color;
    }
}












void display_start_frame(void) {
    // sincronización si es necesaria
}

void display_end_frame(void) {
    // framebuffer al display
    set_window(0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);
    send_cmd(CMD_RAMWR);
    send_data((uint8_t*)frame_buffer, DISPLAY_WIDTH * DISPLAY_HEIGHT * 2);
}














//esto no hace falta le pones brillo maximo 
void display_set_brightness(uint8_t percent) {
    // no se como es el display
    uint8_t brightness = (percent * 255) / 100;
    send_cmd(0x51); // a modificar dependiendo de como quede
    send_data(&brightness, 1);
}

















void display_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    display_draw_pixel(x0, y0 + r, color);
    display_draw_pixel(x0, y0 - r, color);
    display_draw_pixel(x0 + r, y0, color);
    display_draw_pixel(x0 - r, y0, color);

    while(x < y) {
        if(f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        display_draw_pixel(x0 + x, y0 + y, color);
        display_draw_pixel(x0 - x, y0 + y, color);
        display_draw_pixel(x0 + x, y0 - y, color);
        display_draw_pixel(x0 - x, y0 - y, color);
        display_draw_pixel(x0 + y, y0 + x, color);
        display_draw_pixel(x0 - y, y0 + x, color);
        display_draw_pixel(x0 + y, y0 - x, color);
        display_draw_pixel(x0 - y, y0 - x, color);
    }
}



