# AmigaBall LCD demo

## Description

This is a port to OpenPicoRTOS of the excellent AmigaBall demo from slmisc: https://github.com/slmisc/gd32v-lcd

The purpose of this demo is to show the high portability of OpenPicoRTOS, not exceptional performance.   
Currently it runs on the following platforms:
  - SiPeed Longan Nano (GD32VF103)
  - Raspberry Pi Pico (RP2040)
  - Adafruit ItsyBitsy M4 Express (SAMD51)
  - TI TMS320F28379D (C2000/c28x)

## How to build

Just run make -f /platform/.make, where /platform/ is the board you're interested in.

## How to run

### Rapsberry Pico

The Raspberry Pico version generates two UF2 files that can be copied directly for a quick test.
The first file runs from ROM (execute in place) and the second one runs from RAM.

You can use a SWD probe like PicoProbe and directly run the .elf files. OpenOCD config file is in openocd/

### SiPeed Longan Nano

The Longan Nano demo can only run from ROM (XIP) due to its lack of RAM space.   
Irequires a probe or dfu_utils. You can try typing

    # make -f sipeed-longan-nano.make upload

after switching the board to bootloader mode.

### Adafruit Istsy-Bitsy M4 Express

Just like the RPi Pico, the Adafruit build generates 2 files, 1 XIP and one that runs from RAM.
If you have arduino-cli & the necessary extensions, after the build, just type:

    # maker -f adafruit-itsybitsy-m4.make upload

You can use a SWD probe like PicoProbe and directly run the .elf files, too. OpenOCD config file is in openocd/

### TI LAUNCHXL-F28379D

```
LAUNCHXL | 0.96" LCD
   3V3   -    VCC
   GND   -    GND
   P60   -    SCL
   P58   -    SDA
   P22   -    RES
   P105  -    DC
   P104  -    CS
```

## Wiring

### SiPeed Longan Nano

No special wiring is needed, you just need to connect a 160*80 LCD screen to the corresponding port.

### Raspberry Pico

Just connect your Pico to a Waveshare LCD 0.96 interface

### Adafruit ItsyBitsy M4 Express

```
Adafruit | 0.96" LCD
   3V3   -    VCC
   G     -    GND
   MO    -    SDA
   SCK   -    SCL
   A5    -    RST
   A4    -    DC
   A3    -    CS
```

### TI LAUNCHXL-F28379D

```
LAUNCHXL | 0.96" LCD
  3V3    -   VCC
  GND    -   GND
  P58    -   SDA
  P60    -   SCL
  P22    -   RST
  P105   -   DC
  P104   -   CS

```

## Remarks

Currently the Raspberry Pico version is quite slow, ceiling at 25fps. The way this demo is designed does not really take
advantage of the 2 cores, it might even be slower in SMP than in single core mode.

You might want to upload your own solution by splitting the lcd task to distribute the load evenly.   
This is not the scope of this demo but you can have fun, too !
