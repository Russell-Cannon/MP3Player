cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_lvgl_st7789 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# ── LVGL sources ───────────────────────────────────────────────
file(GLOB_RECURSE LVGL_SOURCES
    ${CMAKE_SOURCE_DIR}/lib/lvgl/src/*.c
)

# ── Main executable ────────────────────────────────────────────
add_executable(pico_lvgl_st7789
    main.c
    st7789.c
    lvgl_driver.c
    ${LVGL_SOURCES}
)

# ── Include paths ──────────────────────────────────────────────
target_include_directories(pico_lvgl_st7789 PRIVATE
    ${CMAKE_SOURCE_DIR}              # lv_conf.h lives here
    ${CMAKE_SOURCE_DIR}/lib/lvgl     # lvgl.h lives here
)

# ── LVGL needs to know where lv_conf.h is ─────────────────────
target_compile_definitions(pico_lvgl_st7789 PRIVATE
    LV_CONF_INCLUDE_SIMPLE          # tells LVGL to find lv_conf.h on the include path
)

# ── Pico SDK libraries ─────────────────────────────────────────
target_link_libraries(pico_lvgl_st7789
    pico_stdlib
    hardware_spi
    hardware_gpio
)

pico_enable_stdio_usb(pico_lvgl_st7789 1)
pico_enable_stdio_uart(pico_lvgl_st7789 0)
pico_add_extra_outputs(pico_lvgl_st7789)