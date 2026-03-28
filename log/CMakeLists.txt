cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico2_w CACHE STRING "Target board")
set(PICO_PLATFORM rp2350-arm-s CACHE STRING "Target platform")

# MUST be before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(log C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(log src/log.c)
pico_enable_stdio_usb(log  1)

target_link_libraries(log pico_stdlib)
pico_add_extra_outputs(log)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/blink.uf2 DESTINATION /firmware)
