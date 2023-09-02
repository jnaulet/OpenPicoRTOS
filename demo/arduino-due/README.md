# picoRTOS Arduino Due demo

## Description

This simple demo is a hardware and software test for the Arduino Due platform, it tests:

  - ADCs
  - CANs
  - GPIOs
  - UART
  - I2C
  - PWMs / IPWMs
  - SPI
  - Watchdog

## Requirements

Some wiring is required before you can perform this test.

In order to work properly these connections have to be made:

  - A0 to 3.3V
  - PWM2 to PWM3
  - ICSP connector pin 1 to 4 (SPI hardware loopback)
  - SCL to SCL1 and SDA to SDA1
  - CANTX & CANRX to a CAN transceiver
  - DAC0 (CANRX1) & DIGITAL53 (CANTX1) to another CAN transceiver
  - Loopback between the 2 can transceivers
