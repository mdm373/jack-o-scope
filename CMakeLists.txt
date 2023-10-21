cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(jack_o_scope)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(jack_o_scope)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/.generated)
# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(jack_o_scope ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/.generated)
target_sources(jack_o_scope PRIVATE src/main.c)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(jack_o_scope PRIVATE pico_stdlib hardware_pio)
pico_add_extra_outputs(jack_o_scope)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(jack_o_scope)

# Additionally generate python and hex pioasm outputs for inclusion in the RP2040 datasheet
add_custom_target(pio_ws2812_datasheet DEPENDS ${CMAKE_CURRENT_LIST_DIR}/.generated/ws2812.py)
add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/.generated/ws2812.py
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio
        COMMAND Pioasm -o python ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio ${CMAKE_CURRENT_LIST_DIR}/.generated/ws2812.py
        VERBATIM)
add_dependencies(jack_o_scope pio_ws2812_datasheet)