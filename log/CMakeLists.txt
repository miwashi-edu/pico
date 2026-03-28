cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico2_w CACHE STRING "Target board")
set(PICO_PLATFORM rp2350-arm-s CACHE STRING "Target platform")

# MUST be before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(log C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${PROJECT_NAME} src/log.c
)
pico_enable_stdio_usb(${PROJECT_NAME}  1)

target_link_libraries(${PROJECT_NAME} pico_stdlib)
pico_add_extra_outputs(${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.uf2 DESTINATION /firmware)
