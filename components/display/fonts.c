#include "fonts.h"
#include "display.h"
#include <string.h>
#include "esp_log.h"
#include "driver/spi_master.h"



// funciones
/*
void display_draw_text(const char *text, uint16_t x, uint16_t y, uint8_t color, font_size_t size) {
    const font_t *font = &fonts[size];
    uint16_t current_x = x;
    
    for (const char *c = text; *c; c++) {
        if (*c < font->first_char || *c >= (font->first_char + font->char_count)) {
            continue; 
        }
        
        uint32_t char_offset = (*c - font->first_char) * font->height * ((font->width + 7) / 8);
        const uint8_t *char_data = &font->data[char_offset];

        for (uint8_t row = 0; row < font->height; row++) {
            for (uint8_t col = 0; col < font->width; col++) {
                uint8_t byte_pos = col / 8;
                uint8_t bit_pos = 7 - (col % 8);
                
                if (char_data[row * ((font->width + 7) / 8) + byte_pos] & (1 << bit_pos)) {
                    display_draw_pixel(current_x + col, y + row, color);
                }
            }
        }
        current_x += font->width + 1;
    }
}

*/



// Function to generate a pixel buffer for a single character
// Returns a pointer to the generated buffer, or NULL on error.
// The caller is responsible for freeing the allocated memory.
uint8_t* generate_char_pixel_buffer(char c, const font_t *font, uint8_t text_color, uint8_t background_color) {
    if (c < font->first_char || c >= (font->first_char + font->char_count)) {
        return NULL; // Character not in font
    }

    // Calculate the size of the buffer needed for one character
    // Each pixel is 1 byte
    size_t buffer_size = (size_t)font->width * font->height * 1; // 1 byte per pixel
    uint8_t *pixel_buffer = (uint8_t*)malloc(buffer_size);

    if (pixel_buffer == NULL) {
        return NULL; // Memory allocation failed
    }

    uint32_t char_offset = (c - font->first_char) * font->height * ((font->width + 7) / 8);
    const uint8_t *char_data = &font->data[char_offset];

    uint32_t buffer_idx = 0; // Index for the pixel_buffer

    for (uint8_t row = 0; row < font->height; row++) {
        for (uint8_t col = 0; col < font->width; col++) {
            uint8_t byte_pos = col / 8;
            uint8_t bit_pos = 7 - (col % 8);

            uint8_t pixel_color_rgb332; // This will hold the final 8-bit color
            if (char_data[row * ((font->width + 7) / 8) + byte_pos] & (1 << bit_pos)) {
                pixel_color_rgb332 = text_color; // Pixel is 'on'
            } else {
                pixel_color_rgb332 = background_color; // Pixel is 'off'
            }

            // Store the 8-bit color directly into the buffer
            pixel_buffer[buffer_idx] = pixel_color_rgb332;
            
            buffer_idx++;
        }// para saber cuando termino una fila ESP_LOGI(TAG,"fila num: %u",row);

    }

    //for(uint8_t i = 0;i<sizeof(pixel_buffer);i++){}
    return pixel_buffer;
}

// Now, your display_draw_text would take uint8_t for colors directly:
void display_draw_text_buffered(char *text, uint16_t x, uint16_t y,
                                uint8_t text_color, uint8_t background_color, font_size_t size) {
    const font_t *font = &fonts[size];
    uint16_t current_x = x;

    for (const char *c_ptr = text; *c_ptr; c_ptr++) {
        char current_char = *c_ptr;

        // Pass the 8-bit RGB332 colors directly to the generation function
        uint8_t *char_pixel_buffer = (uint8_t *)heap_caps_malloc(DMA_BUFFER_SIZE_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);//(uint8_t*)malloc(buffer_size);
        //pixel_buffer = char_test;

        char_pixel_buffer = generate_char_pixel_buffer(current_char, font, text_color, background_color);
        
        
        if (char_pixel_buffer == NULL) {
            // Handle error (e.g., character not found or memory allocation failed)
            // You might want to draw a placeholder or simply skip
            ESP_LOGI(TAG,"character not found or memory failed allocation");
            continue;
        }

        // Now, send this buffer to the LCD.
        // Your ILI9844_WriteBitmap function must be configured for 8-bit pixel data.
        display_draw_character_buffered(current_x, y, font->width, font->height, char_pixel_buffer);

        // Free the allocated memory for the character buffer
        //free(char_pixel_buffer);

        current_x += font->width + 1; // Advance for the next character
    }
}



