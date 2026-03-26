include(FetchContent)

FetchContent_Declare(
    pico-sdk
    GIT_REPOSITORY https://github.com/raspberrypi/pico-sdk.git
    GIT_TAG        2.2.0
)
FetchContent_MakeAvailable(pico-sdk)

set(PICO_SDK_PATH ${pico-sdk_SOURCE_DIR})
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
