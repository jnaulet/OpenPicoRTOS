cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

set(PICORTOS_PATH ../..)
include(${PICORTOS_PATH}/devices/raspberry/rp2040/rp2040.cmake)

# Add raspberry-pico example
add_executable(raspberry-pico
  main.c
  raspberry-pico.c
  ${PICORTOS_SRC_FILES}
  ${PICORTOS_PATH}/ipc/picoRTOS_mutex.c
  ${PICORTOS_PATH}/ipc/picoRTOS_cond.c
  ${PICORTOS_PATH}/drivers/adc/adc-rp2040.c
  ${PICORTOS_PATH}/drivers/clock/clock-rp2040.c
  ${PICORTOS_PATH}/drivers/gpio/gpio-sio.c
  ${PICORTOS_PATH}/drivers/mux/mux-rp2040.c
  ${PICORTOS_PATH}/drivers/pwm/pwm-rp2040.c
  ${PICORTOS_PATH}/drivers/spi/spi-pl022.c
  ${PICORTOS_PATH}/drivers/uart/uart-pl011.c
  ${PICORTOS_PATH}/drivers/twi/twi-dw_apb_i2c.c
  ${PICORTOS_PATH}/drivers/wd/wd-rp2040.c
  )

# picoRTOS includes
target_include_directories(raspberry-pico PRIVATE
  .
  ${PICORTOS_INCLUDE_DIRS}
  ${PICORTOS_PATH}/drivers/include
  ${PICORTOS_PATH}/drivers/adc
  ${PICORTOS_PATH}/drivers/clock
  ${PICORTOS_PATH}/drivers/gpio
  ${PICORTOS_PATH}/drivers/mux
  ${PICORTOS_PATH}/drivers/pwm
  ${PICORTOS_PATH}/drivers/spi
  ${PICORTOS_PATH}/drivers/twi
  ${PICORTOS_PATH}/drivers/uart
  ${PICORTOS_PATH}/drivers/wd
  )

# pull in common dependencies
target_link_libraries(raspberry-pico
  pico_stdlib
  )

# create map/bin/hex file etc.
pico_add_extra_outputs(raspberry-pico)

add_compile_options(-Wall
  -Wextra
  )

