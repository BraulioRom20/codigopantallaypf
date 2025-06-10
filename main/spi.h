#pragma once
#include "driver/spi_master.h"

void spi_init(void);
spi_device_handle_t get_spi_handle(void);