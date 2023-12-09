# picoRTOS NXP MPC5775B/E single core / SMP demo

## Description

This simple demo uses the mpc5775b/e-evb & tests the following:

  - ADC
  - CAN
  - GPIO
  - MUX
  - PWM
  - SPI

## Requirements

Some wiring is required to perform this standalone test:

  - J125, PIN1 (eMIOS0) to D20 for GPIO
  - J125, PIN2 (eMIOS1) to D21 for GPIO
  - J125, PIN3 (eMIOS2) to D22 for PWM
  - J125, PIN12 (eMIOS11) to J125, PIN14 (eMIOS13) for SPI loopback

## How to build

To build for all targets, type:

    # make
