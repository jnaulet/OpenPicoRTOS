# picoRTOS ATTiny817 XPlained Mini demo

## Description

This simple demo is a hardware and software test for the ATTIny817 platform, it can test:

  - ADC
  - TCA0 PWM(s)
  - GPIOs
  - UART
  - SPI
  - Watchdog

Due to the obvious lack of RAM (512bytes...) not all peripherals can be tested at once.
The makefile offers 3 configs:
  - make config1 -> GPIO/LED, UART/CONSOLE, Watchdog
  - make config2 -> GPIO/LED, UART/CONSOLE, SPI
  - make config3 -> GPIO/LED, ADC, PWM

## Requirements

Some wiring is required before you can perform this test.

In order to work properly these connections have to be made:

  - PA4 to 3.3V (ADC test)
  - PA1 to PA2 (SPI loopback)
