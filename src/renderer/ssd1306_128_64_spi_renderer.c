#include "ssd1306_128_64_spi_renderer.h"

void ssd1306_128_64_spi_renderer_init(SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, 
	spi_inst_t *spi, uint32_t baudrate, uint8_t sclk_pin, uint8_t mosi_pin, uint8_t dc_pin, 
	uint8_t rst_pin, uint8_t cs_pin)
{
    ssd1306_init(&(ssd1306_128_64_spi_renderer->driver), SSD1306_128_64_SPI_DISPLAY_WIDTH, 
        SSD1306_128_64_SPI_DISPLAY_HEIGHT, spi, baudrate, sclk_pin, mosi_pin, 
        dc_pin, rst_pin, cs_pin, ssd1306_128_64_spi_renderer->driver_buffer);
}

extern void ssd1306_128_64_spi_renderer_render_image(SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, 
	uint8_t image[SSD1306_128_64_SPI_DISPLAY_HEIGHT * SSD1306_128_64_SPI_DISPLAY_WIDTH])
{
    uint8_t i, j;
    
    for (i = 0; i < SSD1306_128_64_SPI_DISPLAY_HEIGHT; ++i) {
        for (j = 0; j < SSD1306_128_64_SPI_DISPLAY_WIDTH; ++j) {
            if (image[i * SSD1306_128_64_SPI_DISPLAY_WIDTH + j])
                ssd1306_draw_pixel(&ssd1306_128_64_spi_renderer->driver, j, i, SSD1306_COLOR_ON);
            else
                ssd1306_draw_pixel(&ssd1306_128_64_spi_renderer->driver, j, i, SSD1306_COLOR_OFF);
		}
	}
    ssd1306_update(&ssd1306_128_64_spi_renderer->driver);
}