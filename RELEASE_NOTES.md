# Release notes

## picoRTOS v1.9.8
### What's new ?

Major update, experimental support for NXP HC(S)08 architecture.

Added a perl preprocessor to generate the .S files from C header file on SDCC-related builds.   
Which leads to minor updates to 8051 port, as .inc files were not needed anymore.

A few drivers have been developped for the S08PTxx series to allow tests in staging directory.

## picoRTOS v1.9.7
### What's new ?

Minor update, support for a new architecture:

    - RISC-V: support for RV32EC + WCH CH32V003
    - CH32V003 minimal drivers (clock, mux, gpio) + demo

## picoRTOS v1.9.6
### What's new ?

This is a minor update, but kinda major in some ways.

    - 8051: improved architecture + clock management system(s)
    - AVR: total refactoring of the ports (merged ATMegza & ATTiny) + associated demos
    - STM32F10xxx: support for a new ARMV7-M machine + minimal drivers (clock, mux, gpio)
    - STM32F401x: support for new ARMv7e-M machine + minimal drivers (clock)
    - Refactoring of DMA setup in existing SPI drivers (to match +irqdriven naming conventions)

## picoRTOS v1.9.5
### What's new ?

The biggest improvement of this release is the support for the Intel 8051 / MCS51 family of microcontrollers and the associated support
for SDCC as a compiler.

Code for ucsim_51, Nuvoton n76e003 & Cypress/Infineon Cy7c6801xA chips is provided as an example. This is still incomplete, though, as
no driver (except for the clocks) has been provided, only the kernel runs.

Major fixes have been added to the SMP ports (main cores are now waiting until the auxiliary ones are idling before running the scheduler).
Chips that do not support bus snooping are now using the mutexes properly.

Finally, 8bits int types have been used whenever possible on 8bit architectures (AVR & 8051).

## picoRTOS v1.9.4
### What's new ?

The HAL & associated drivers have been modified to remove more booleans and make the structure inits more human-readable.

NXP Devkit-MPC5748G has been unstaged after MUX, GPIO, ADC, PWM, FlexCAN, UART (LinFlexD), SPI/DSPI & clock drivbers has been roughly tested.

NXP MPC5775B/E demo has been improved to include drivers from MPC574xx series. DMA, GPIO, MUX & clock have been unstaged, too.

Check & distcheck ahve been run & improved (especially the MIPS-related ones).

## picoRTOS v1.9.3
### What's new ?

Two new ports have been added: VLE PowerPCs e200z4 & e200z7.   
Both support single core & dual core SMP configurations.

The corresponding demos & drivers have been updated to the current APIs & stay for the
most part totally untested. This will be the next step.

## picoRTOS v1.9.2
### What's new ?

The main new feature in this release is the management of the CPU clock frequency inside the
ports, which allow new API functions like arch_delay_us with decent accuracy.

Clock drivers are responsible for setting this correctly & have been updated.
From the user's perspective it's one less parameter to worry about
(except for the AVR architecture).

Tasks statistics have been improved without adding too much overhead and in was a real PITA !
I hope it's the final version.

## picoRTOS v1.9.1
### What's new ?

This version adds support for Renesas RA4 series in general & Arduino Uno R4 Minima in particular.   
ADC, Clocks, GPIOs, Multiplexing, PWMs, SPIs & UARTs are supported in quite a minimalistic way.

TWI/I2C needs further testing (code is available in the staging tree).
Other peripherals are not supported yet.

On the minor side, the auto-generated config can now be included simply by including picoRTOS.h

## picoRTOS v1.9.0
### What's new ?

This is a major revision that brings up a brand new graphical configuration menu (GUI), heavily
inspired by the Linux/U-boot/Buildroot "make menuconfig".

OpenPicoRTOS is now more than an OS. It is an OS AND a build system AND a code checking system, too.

Migrating to OpenPicoRTOS has been made easier than ever (hopefully).

Some changes have been made to the port API & the main API, especially regarding asserts, that have been
made ISO-C compliant to accomodate for the now default gcc -pedantic mode.

Cross-dependencies between the ports and the schedulers have been totally removed.

All drivers have benn migrated already & some have been improved, especially the SAM3x/Arduino Due I2C &
CAN.

Some work still needs to be done on the PowerPC chips, as we are still waiting for some replacements
after the last ones have been stolen.

## picoRTOS v1.8.4
### What's new ?

Support for ARM Cortex-M7 has been added, as well as some support for the stm32h742/750 series
provided by DevEBox for its cheap chinese eval boards.

Some refactoring has been done on the ARM linker files, but it's still not 100% satisfying.

## picoRTOS v1.8.3
### What's new ?

Mostly, the AmigaBall LCD demo, courtesy of slmisc on github.
This demo has allwoed to put a series of board to a real test & imporve on the DMA for most of
them (except c2000, which is a pain anyway).

A more satsifying way to manage caches has been found, hopefully.

PowerPC plaftorms have been added to staging and deserve a huge amount of testing.

## picoRTOS v1.8.2
### What's new ?

The main change in this release is the support for RISC-V RV32IMAC architectures.
This has been tested on the GD32VF103 (Bumblebee core) & 2 demos are provided: one for the peripherals
and one for fun (LCD).

A bunch of drivers has been partially tested & moved out of staging for this device.

A new concept has been added, architecture drivers. This is mostly for interrupt controllers.   
An example is provided as the support for intc-pic32mx in the mips32/m51xx architecture directory.

Raspberry Pico demos (single-core and SMP) have been merged to avoid too much code duplication.

Tests have been improved, now the static analysis happens on all available devices, not only
the architectures.

## picoRTOS v1.8.1
### What's new ?

OpenPicoRTOS now supports software cache synchronization through new
interfaces in the ports new arch_invalidate_dcache() and arch_flush_dcache().
These have not effect on CPUs with no cache.

Casting types in drivers inits have been deemed unnecessary & misleading and
have been converted to integers to allow direct use of defines in
picoRTOS_device.h.

Some small bug fixes have been added to the mix, as well as some minor
structural improvements in the schedulers, especially syscalls.

## picoRTOS v1.8.0
### What's new ?

OpenPicoRTOS is now a standalone kernel that only needs a toolchain to be
built.   
This allows it to run from flash or from ram depending on the level of
performance your project requires.   
This makes ports a little more challenging in the beginning but gives then a lot
of freedom at runtime.

A complete static analysis with splint & cppcheck has been performed on
the code to ensure it was free of errors & some minor bugfixes have followed.

Two new drivers for flash controllers have been added :
  - SAMD/E5x
  - PIC32Mx

The flash HAL is still experimental and probably requires some improvements
but at least the mechanics could be tested, finally.

arch/avr/avr5 & arch/avr/avr6 have been merged in a more convenient
arch/avr/avrmega

A new port is available: arch/c2000/c28x. A demo for the launchxl-f28379d eval
board is provided to test the scheduler & the drivers.

Finally, a staging tree has been added. It mimics the main tree but only contains
experimental features / drivers / ports that could not be tested for one reason or
the other, but fulfil the code safety requirements (2 static analysers + build with
no errors or warnings).

A RISC-V (RV32IMAC) port is staging, alongside general support for the
GigaDevice GD32VF103 microcontroller.

## picoRTOS v1.7.0
### What's new ?

Round-robin scheduling ! This is the new feature provided in the v1.7.x series.
Works in SMP & single core modes.

Some drivers have been moved around:
  - spi-pl022 -> spi-arm_pl022
  - uart-pl011 -> uart-arm_pl011
  - gpio-sio -> gpio-rp2040_sio

Asserts have changed a bit for testing purposes, you get now:
 - assert_fatal (returns)
 - assert_void (only in debug)
 - assert_void_fatal (doesn't return)

Unit tests have been added to ensure single-core & SMP versions get the same behaviour.


## picoRTOS v1.6.3
### What's new ?

The system has been partially rewritten to prepare the field for the future support of round-robin scheduling.
The PIC32MZ series support has been improved, in the scheduler as well as the drivers.
the can_accept() HAL call and the associated drivers have been improved (no more exceptions for 0).

The following drivers have been added:
  - Watchdog for SAMD5x/E5x series
  - ADC for PIC32Mx series
  - TWI/I2C for PIC32Mx (untested)
  - CAN for PIC32Mx (untested)

## picoRTOS v1.6.2
### What's new ?

Support has been added for the following architectures:
  - ARM Cortex M4/F
  - MIPS M51xx

This support is incomplete as far as the MIPS is concerned (no support for FPU or DSP yet).
This is on purpose, as the split between architecture ankd BSP is not completely decided yet.

The follwing devices are now at least partially supported:
  - Atmel SAMD5x/E5x
  - Atmel TinyAVR ATtiny1607
  - PIC32MZ-EF

Basic drivers are provided for these platforms, SAMD5x and ATtiny have good support, PIC32 is fairly incomplete.

## picoRTOS v1.6.1
### What's new ?

Support has been added for Atmel tinyAVR 1-series. Some drivers and test demo have been added to
the project.

AVR assembly ports have been simplified to the extreme using .rept macros

A shortcut macro for stacks has been added: PICORTOS_STACK_COUNT(x)

Queues are now fully thread-safe.

## picoRTOS v1.6.0
### What's new ?

A lot ! picoRTOS now offers a HAL and a bunch of experimental drivers for all
supported platforms.

Demos have been updated to reflect that and show the power of this new
code-level HAL.

## picoRTOS v1.5.4
### What's new ?

picoRTOS is back ! Version 1.5.3 has seen the project being retired for professionnal use, so this
one is a brand new architecture that has been re-written from the ground up.

Unfortunately older users of picoRTOS on PowerPC and C2000 cannot update because despite similar
user interfaces, the port API has been fundamentally rewritten and is not compatible anymore.

Anyway, some features left us, but new features have been added, like "dynamic" priority assignment,
syscalls, queues and much more.

picoRTOS is now safer, faster and more complete, enjoy !
