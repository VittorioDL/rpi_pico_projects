cmake_minimum_required(VERSION 3.20.0)

set(BOARD rpi_pico)
#set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/rpi_pico.overlay)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(prova_led)

target_sources(app PRIVATE src/main.c)


