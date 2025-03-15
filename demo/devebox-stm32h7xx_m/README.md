# picoRTOS STM32H7xx demo (Devebox AliExpress board)

## Description

This simple demo uses the STM32H7xx & tests the following:

  - GPIO
  - UART
  - PWM
  - SPI
  - CAN
  - RNG

## Requirements

Some wiring is required to perform this standalone test:

  - PA6 to PA7 (SPI MOSI/MISO)
  - PB8 to PB9 (UART RX/TX)

## How to build

To build for single-core RAM & XIP, type:

    # make
