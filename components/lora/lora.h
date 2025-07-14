//libreria hecha con spi para LoRA xl1278 esp-idf 5.4

//only pins of the lora module.
#define PIN_SPI_CS_LORA   32
#define PIN_SPI_DC_LORA   34
#define PIN_SPI_RST_LORA  35
//#define PIN_SPI_MISO 33

//clk y mosi comparte con el bus del display tft 3.5

//funciones lora:
void lora_spi_init();
void lora_reset();
uint8_t lora_read_register(uint8_t reg);
void lora_write_register(uint8_t adress, uint8_t payload);
void lora_send_packet(const char *data);
void lora_init();
void show_lora_version();