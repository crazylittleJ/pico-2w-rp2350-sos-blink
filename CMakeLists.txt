cmake_minimum_required(VERSION 3.13)

# Set (or export as environment variables) before configuring:
#   PICO_SDK_PATH        -> path to the pico-sdk repo
#   FREERTOS_KERNEL_PATH  -> path to the raspberrypi/FreeRTOS-Kernel repo (note: NOT the
#                            official FreeRTOS/FreeRTOS-Kernel repo, which does not yet
#                            have an RP2350_ARM_NTZ port and will fail at configure time)

set(PICO_BOARD pico2_w)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

project(sos_blink C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(sos_blink
    main.c
)

target_include_directories(sos_blink PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# pico_cyw43_arch_none: only uses the cyw43 chip's GPIO (including the onboard LED),
# does not enable the wifi driver stack.
# On the Pico 2 W, the onboard LED is wired to the CYW43439 wifi chip, not to an MCU
# GPIO pin, so it can't be driven with pico_stdlib's gpio_put() — you have to go
# through cyw43_arch_gpio_put() instead.
target_link_libraries(sos_blink
    pico_stdlib
    pico_cyw43_arch_none
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_add_extra_outputs(sos_blink)
