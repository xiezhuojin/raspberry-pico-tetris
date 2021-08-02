#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/sem.h"

#include "predefined_object/falling_piece.h"
#include "predefined_object/tetris_image.h"
#include "core/playground_32_16.h"
#include "renderer/ssd1306_128_64_spi_renderer.h"
#include "component_renderer_mapping.h"
#include "parameter.h"


void game_over();
void current_falling_piece_init();

void init_game();
void start_button_pushed_callback(uint gpio, uint32_t events);

void left_button_pushed_callback();
void right_button_pushed_callback();
void down_button_pushed_callback();
void rotate_90_degree_button_pushed_callback();
void on_game_button_pushed_callback(uint gpio, uint32_t events);
bool fall_timeout_callback(repeating_timer_t *rt);


semaphore_t sem;
struct repeating_timer rt;

SSD1306_128_64_SPI_Renderer ssd1306_128_64_spi_renderer;
Playground_32_16 playground_32_16;
FallingPiece *candidate_falling_pieces[] = {
    &falling_piece_1_1,
    &falling_piece_2_1,
    &falling_piece_3_1, &falling_piece_3_2,
    &falling_piece_4_1, &falling_piece_4_2, &falling_piece_4_3, &falling_piece_4_4, &falling_piece_4_5
};
unsigned int n_candidate_falling_pieces = sizeof(candidate_falling_pieces) / sizeof(FallingPiece *);
FallingPiece *current_falling_piece;

void game_over() {
    gpio_set_irq_enabled_with_callback(TETRIS_LEFT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_RIGHT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_DOWN_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_ROTATE_90_DEGREE_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);

    cancel_repeating_timer(&rt);

    ssd1306_128_64_spi_renderer_render_image(&ssd1306_128_64_spi_renderer, tetris_start_128_64_image);
}

void current_falling_piece_init() {
    unsigned char i, j;
    unsigned int n_fallen_piece_point;

    for (i = 0; i < playground_32_16.meta_playground.height; ++i)
        for (j = 0; j < playground_32_16.meta_playground.width; ++j)
            if (playground_32_16.rows[i][j])
                n_fallen_piece_point += (i + j);
    current_falling_piece = candidate_falling_pieces[n_fallen_piece_point % n_candidate_falling_pieces];

    current_falling_piece->pivot_relative_position.x = playground_32_16.meta_playground.width / 2;
    current_falling_piece->pivot_relative_position.y = playground_32_16.meta_playground.height + current_falling_piece->n_point;
}

void init_game() {
    sem_init(&sem, 1, 1);

    ssd1306_128_64_spi_renderer_init(&ssd1306_128_64_spi_renderer, spi0, 8000 * 1000, 2, 3, 4, 5, 6);
    ssd1306_start(&ssd1306_128_64_spi_renderer.driver);
    ssd1306_128_64_spi_renderer_render_image(&ssd1306_128_64_spi_renderer, tetris_start_128_64_image);

    // press any button to start
    gpio_set_irq_enabled_with_callback(TETRIS_LEFT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_RIGHT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_DOWN_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_ROTATE_90_DEGREE_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, start_button_pushed_callback);
}

void start_button_pushed_callback(uint gpio, uint32_t events) {
    playground_32_16_init(&playground_32_16);
    current_falling_piece_init();

    ssd1306_clear(&ssd1306_128_64_spi_renderer.driver);
    ssd1306_update(&ssd1306_128_64_spi_renderer.driver);

    // gpio_set_irq_enabled_with_callback does not support a callback per gpio pin
    gpio_set_irq_enabled_with_callback(TETRIS_LEFT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, on_game_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_RIGHT_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, on_game_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_DOWN_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, on_game_button_pushed_callback);
    gpio_set_irq_enabled_with_callback(TETRIS_ROTATE_90_DEGREE_BUTTON_GPIO, GPIO_IRQ_EDGE_RISE, 1, on_game_button_pushed_callback);
    alarm_pool_add_repeating_timer_ms(alarm_pool_get_default(), FALLING_PIECE_FALL_TIMEOUT_MS, fall_timeout_callback, NULL, &rt);
}

void left_button_pushed_callback() {
    ssd1306_128_64_spi_renderer_unrender_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
    falling_piece_left_shift(current_falling_piece);
    if (falling_piece_collide_with_playground(current_falling_piece, &playground_32_16.meta_playground))
        falling_piece_right_shift(current_falling_piece);
    ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
}

void right_button_pushed_callback() {
    ssd1306_128_64_spi_renderer_unrender_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
    falling_piece_right_shift(current_falling_piece);
    if (falling_piece_collide_with_playground(current_falling_piece, &playground_32_16.meta_playground))
        falling_piece_left_shift(current_falling_piece);
    ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
}

void down_button_pushed_callback() {
    unsigned char collide_value;

    ssd1306_128_64_spi_renderer_unrender_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
    falling_piece_down_shift(current_falling_piece);
    if (falling_piece_collide_with_playground(current_falling_piece, &playground_32_16.meta_playground)) {
        falling_piece_up_shift(current_falling_piece);
        if (falling_piece_above_playground(current_falling_piece, &(playground_32_16.meta_playground)))
            game_over();
        else {
            project_falling_piece_to_playground_fallen_piece(current_falling_piece, &playground_32_16.meta_playground);
            clear_playground_32_16_contiguous_fallen_piece_row(current_falling_piece, &playground_32_16);
            ssd1306_128_64_spi_renderer_render_playground_32_16(&ssd1306_128_64_spi_renderer, &playground_32_16);
            current_falling_piece_init();
        }
    }
    else
        ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
}

void rotate_90_degree_button_pushed_callback() {
    ssd1306_128_64_spi_renderer_unrender_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
    falling_piece_rotate_90_degree(current_falling_piece);
    if (falling_piece_collide_with_playground(current_falling_piece, &playground_32_16.meta_playground))
        falling_piece_rotate_270_degree(current_falling_piece);
    ssd1306_128_64_spi_renderer_render_falling_piece(&ssd1306_128_64_spi_renderer, current_falling_piece);
}

void on_game_button_pushed_callback(uint gpio, uint32_t events) {
    if (! sem_acquire_timeout_ms(&sem, 50))
        return;

    switch (gpio)
    {
    case TETRIS_LEFT_BUTTON_GPIO:
        left_button_pushed_callback();
        break;

    case TETRIS_RIGHT_BUTTON_GPIO:
        right_button_pushed_callback();
        break;

    case TETRIS_DOWN_BUTTON_GPIO:
        down_button_pushed_callback();
        break;

    case TETRIS_ROTATE_90_DEGREE_BUTTON_GPIO:
        rotate_90_degree_button_pushed_callback();
        break;
    }

    sem_release(&sem);
}

bool fall_timeout_callback(repeating_timer_t *rt) {
    on_game_button_pushed_callback(TETRIS_DOWN_BUTTON_GPIO, 0);
}

void main()
{
    init_game();

    while (1);
}