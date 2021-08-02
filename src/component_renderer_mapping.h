#ifndef COMPONENT_RENDERER_MAPPING
#define COMPONENT_RENDERER_MAPPING

#include "renderer/ssd1306_128_64_spi_renderer.h"
#include "core/playground_32_16.h"

extern void ssd1306_128_64_spi_renderer_render_playground_32_16(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, Playground_32_16 *playground_32_16);
extern void ssd1306_128_64_spi_renderer_render_falling_piece(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, FallingPiece *falling_piece);
extern void ssd1306_128_64_spi_renderer_unrender_falling_piece(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, FallingPiece *falling_piece);

#endif