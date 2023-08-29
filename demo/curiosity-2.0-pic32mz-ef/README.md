# picoRTOS Curiosity PIC32MZ-EF demo

## Description

This simple demo tests the following

  - ADC
  - CAN (WIP)
  - GPIO
  - I2C
  - PWM
  - SPI
  - Watchdog

## Requirements

Some wiring is required to perform this standalone test:

  - MISO to MOSI (any mikro BUS)
  - I2C1 to I2C2 (bottom of the board):
    - R751 to R8
    - R749 to R748

## MPLABX

You can run this demo in MPLABX by creating a new project and inserting the
info from the make command into the configurations.xml file of the project.

It would be nicer to completely generate it but i leave that for another day.

Quit MPLABX, re-open it, right-click the project and at XC32 (Global Options), sleect "Override default device support: Compiler location".

Then, in xc32-ld, menu "Libraries", select "Exclude standard libraries" & "Do no link crt0 startup code".

You should be able to use MPLANBX to debug the project now.   
Making the CAN work might be a good idea ;)
