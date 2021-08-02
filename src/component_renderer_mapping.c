#include "component_renderer_mapping.h"
#include "core/abstract_component.h"

#define POINT_SCALE_FACTOR	4

static void ssd1306_128_64_spi_renderer_draw_point(
	SSD1306 *ssd1306, Point *point, uint8_t color);

static void _ssd1306_128_64_spi_renderer_render_falling_piece(
	SSD1306 *ssd1306, FallingPiece *falling_piece, uint8_t color);

void ssd1306_128_64_spi_renderer_render_playground_32_16(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, Playground_32_16 *playground_32_16)
{
	unsigned int i, j;
	Point point;

	for (i = 0; i < PLAYGROUND_32_16_HEIGHT; ++i)
		for (j = 0; j < PLAYGROUND_32_16_WIDTH; ++j) {
			point.x = j;
			point.y = i;
			if (playground_32_16->rows[i][j])
				ssd1306_128_64_spi_renderer_draw_point(&ssd1306_128_64_spi_renderer->driver, &point, SSD1306_COLOR_ON);
			else
				ssd1306_128_64_spi_renderer_draw_point(&ssd1306_128_64_spi_renderer->driver, &point, SSD1306_COLOR_OFF);
		}
	ssd1306_update(&ssd1306_128_64_spi_renderer->driver);
}

void ssd1306_128_64_spi_renderer_render_falling_piece(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, FallingPiece *falling_piece)
{
	_ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer->driver, falling_piece, SSD1306_COLOR_ON);
}

void ssd1306_128_64_spi_renderer_unrender_falling_piece(
	SSD1306_128_64_SPI_Renderer *ssd1306_128_64_spi_renderer, FallingPiece *falling_piece)
{
	_ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer->driver, falling_piece, SSD1306_COLOR_OFF);
}

static void ssd1306_128_64_spi_renderer_draw_point(SSD1306 *ssd1306, Point *point, uint8_t color)
{
	unsigned int i, j;
	signed int x, y;

	for (i = 0; i < POINT_SCALE_FACTOR; ++i)
		for (j = 0; j < POINT_SCALE_FACTOR; ++j) {
			if (point->x >= 0 && point->x < PLAYGROUND_32_16_WIDTH && point->y >= 0 && point->y < PLAYGROUND_32_16_HEIGHT) {
				x = point->x * POINT_SCALE_FACTOR + i;
				y = (PLAYGROUND_32_16_HEIGHT - 1 - point->y) * POINT_SCALE_FACTOR + j;
				ssd1306_draw_pixel(ssd1306, x, y, color);
			}
		}
}

static void _ssd1306_128_64_spi_renderer_render_falling_piece(
	SSD1306 *ssd1306, FallingPiece *falling_piece, uint8_t color)
{
	unsigned int i;
	Point point;

	for (i = 0; i < falling_piece->n_point; ++i) {
		point.x = falling_piece->point[i].x + falling_piece->pivot_relative_position.x;
		point.y = falling_piece->point[i].y + falling_piece->pivot_relative_position.y;
		ssd1306_128_64_spi_renderer_draw_point(ssd1306, &point, color);
	}
	ssd1306_update(ssd1306);
}
