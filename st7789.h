#pragma once
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define ST7789_SPI_PORT spi0
#define ST7789_CLK  4
#define ST7789_MOSI 3
#define ST7789_CS   5
#define ST7789_DC   6
#define ST7789_RST  7
#define ST7789_BL   8   // or wire to 3.3V

#define ST7789_WIDTH  320
#define ST7789_HEIGHT 240

void st7789_init(void);
void st7789_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void st7789_write_pixels(uint16_t *pixels, uint32_t count);