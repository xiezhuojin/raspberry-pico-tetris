#ifndef RENDER_H
#define RENDER_H

#include "driver/raspberry_pi_pico/ssd1306.h"

#define SSD1306_128_64_SPI_DISPLAY_WIDTH	128
#define SSD1306_128_64_SPI_DISPLAY_HEIGHT	64

typedef struct
{
	SSD1306 driver;
	uint8_t driver_buffer[ssd1306_buffer_size(SSD1306_128_64_SPI_DISPLAY_WIDTH, SSD1306_128_64_SPI_DISPLAY_HEIGHT)];
} SSD1306_128_64_SPI_Renderer;

extern void ssd1306_128_64_spi_renderer_init(SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, 
	spi_inst_t *spi, uint32_t baudrate, uint8_t sclk_pin, uint8_t mosi_pin, uint8_t dc_pin, 
	uint8_t rst_pin, uint8_t cs_pin);

extern void ssd1306_128_64_spi_renderer_render_image(SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, 
	uint8_t image[SSD1306_128_64_SPI_DISPLAY_HEIGHT * SSD1306_128_64_SPI_DISPLAY_WIDTH]);

#endif
