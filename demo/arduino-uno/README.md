# picoRTOS Arduino Uno demo

## Description

This simple demo tests the following:

  - ADC
  - EEPROM
  - GPIO
  - I2C
  - PWM
  - SPI
  - UART
  - Watchdog

## Requirements

Some wiring is required before you can perform this test.

In order to work properly these connections have to be made:

  - A0 to 3.3V
  - ICSP connector pin 1 to 4 (SPI hardware loopback)
  - I2C acts like a slave and will answer 0x5a if it received 0xa5 on addr 0x69
