# picoRTOS Curiosiy Nano ATTiny1607 demo

## Description

This simple demo is a hardware and software test for the ATTIny1607 platform, it can test:

  - ADC
  - TCA0 PWM(s)
  - GPIOs
  - UART
  - SPI
  - Watchdog

## Requirements

Some wiring is required before you can perform this test.

In order to work properly these connections have to be made:

  - PA4 to VTG (ADC test)
  - PC1 to PC2 (SPI loopback)
  - PB5 to PB7 (LED PWM test)
