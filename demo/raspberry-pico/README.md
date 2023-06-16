# picoRTOS Raspberry Pico single core / SMP demo

## Description

This simple demo uses the Rpi Pico & tests the following:

  - ADC
  - GPIO
  - UART
  - I2C
  - PWM
  - SPI
  - Watchdog

## Requirements

Some wiring is required to perform this standalone test:

  - GP20 to GP6 (SDA)
  - GP21 to GP7 (SCL)
  - GP27 to GP5 (IPWM)

## How to build

To build for single-core, type:

    # make

To build with support for SMP, type:

    make SMP=1
