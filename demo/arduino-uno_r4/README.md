# picoRTOS Arduino Uno R4 Minima demo

## Description

This simple demo tests the following:

  - ADC
  - GPIO
  - I2C
  - PWM
  - SPI
  - UART

## Requirements

Some wiring is required before you can perform this test.

In order to work properly these connections have to be made:

  - A0 to 3.3V
  - RX <- 0 to TX -> 1
  - I2C acts like a slave and will answer 0x5a if it received 0xa5 on addr 0x69
