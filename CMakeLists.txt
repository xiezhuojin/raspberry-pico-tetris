# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/aleclofquist/repos/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(tetris C CXX ASM)

# Initialise the Pico SDK
pico_sdk_init()

set(SRC src)

# add include directories
include_directories(${SRC})

add_compile_options(-g3)
# Add executable. Default name is the project name, version 0.1
add_executable(tetris 
        ${SRC}/main.c
        # ${SRC}/tetris.c
        ${SRC}/component_renderer_mapping.c
        ${SRC}/renderer/ssd1306_128_64_spi_renderer.c
        ${SRC}/renderer/driver/raspberry_pi_pico/ssd1306.c
        ${SRC}/core/abstract_component.c
        ${SRC}/core/playground_32_16.c
        ${SRC}/predefined_object/falling_piece.c
        ${SRC}/predefined_object/tetris_image.c
        )

pico_set_program_name(tetris "tetris")
pico_set_program_version(tetris "0.1")

pico_enable_stdio_uart(tetris 0)
pico_enable_stdio_usb(tetris 1)

# Add the standard library to the build
target_link_libraries(tetris pico_stdlib)

# Add any user requested libraries
target_link_libraries(tetris
        hardware_spi
        hardware_dma
        hardware_pio
        hardware_timer
        hardware_clocks
        )

pico_add_extra_outputs(tetris)
