#include "st7789.h"
#include "pico/stdlib.h"
#include <string.h>

static void write_cmd(uint8_t cmd) {
    gpio_put(ST7789_DC, 0);
    gpio_put(ST7789_CS, 0);
    spi_write_blocking(ST7789_SPI_PORT, &cmd, 1);
    gpio_put(ST7789_CS, 1);
}

static void write_data(const uint8_t *data, size_t len) {
    gpio_put(ST7789_DC, 1);
    gpio_put(ST7789_CS, 0);
    spi_write_blocking(ST7789_SPI_PORT, data, len);
    gpio_put(ST7789_CS, 1);
}

static void write_data_byte(uint8_t b) { write_data(&b, 1); }

void st7789_init(void) {
    spi_init(ST7789_SPI_PORT, 10000000); // 10 MHz
    gpio_set_function(ST7789_CLK,  GPIO_FUNC_SPI);
    gpio_set_function(ST7789_MOSI, GPIO_FUNC_SPI);

    gpio_init(ST7789_CS);  gpio_set_dir(ST7789_CS,  GPIO_OUT); gpio_put(ST7789_CS, 1);
    gpio_init(ST7789_DC);  gpio_set_dir(ST7789_DC,  GPIO_OUT);
    gpio_init(ST7789_RST); gpio_set_dir(ST7789_RST, GPIO_OUT);
    gpio_init(ST7789_BL);  gpio_set_dir(ST7789_BL,  GPIO_OUT); gpio_put(ST7789_BL, 1);

    // Hardware reset
    gpio_put(ST7789_RST, 1); sleep_ms(10);
    gpio_put(ST7789_RST, 0); sleep_ms(10);
    gpio_put(ST7789_RST, 1); sleep_ms(150);

    write_cmd(0x01); sleep_ms(150); // SWRESET
    write_cmd(0x11); sleep_ms(255); // SLPOUT
    write_cmd(0x3A); write_data_byte(0x55); // COLMOD: 16-bit RGB565
    write_cmd(0x36); write_data_byte(0x00); // MADCTL
    // Set display dimensions
    write_cmd(0x2A); // CASET
    uint8_t caset[] = {0x00,0x00, (ST7789_WIDTH-1)>>8, (ST7789_WIDTH-1)&0xFF};
    write_data(caset, 4);
    write_cmd(0x2B); // RASET
    uint8_t raset[] = {0x00,0x00, (ST7789_HEIGHT-1)>>8, (ST7789_HEIGHT-1)&0xFF};
    write_data(raset, 4);
    write_cmd(0x21); // INVON (ST7789 needs inversion)
    sleep_ms(10);
    write_cmd(0x29); // DISPON
    sleep_ms(255);
}

void st7789_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    write_cmd(0x2A);
    uint8_t caset[] = {x0>>8, x0&0xFF, x1>>8, x1&0xFF};
    write_data(caset, 4);
    write_cmd(0x2B);
    uint8_t raset[] = {y0>>8, y0&0xFF, y1>>8, y1&0xFF};
    write_data(raset, 4);
    write_cmd(0x2C); // RAMWR
}

void st7789_write_pixels(uint16_t *pixels, uint32_t count) {
    gpio_put(ST7789_DC, 1);
    gpio_put(ST7789_CS, 0);
    // Swap bytes for correct endianness
    for (uint32_t i = 0; i < count; i++) {
        uint8_t hi = pixels[i] >> 8;
        uint8_t lo = pixels[i] & 0xFF;
        spi_write_blocking(ST7789_SPI_PORT, &hi, 1);
        spi_write_blocking(ST7789_SPI_PORT, &lo, 1);
    }
    gpio_put(ST7789_CS, 1);
}