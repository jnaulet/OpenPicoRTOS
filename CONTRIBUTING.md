# Contributing to picoRTOS

In order to contribute to picoRTOS, here are the steps to follow:

## Step 1

Make your port in the corresponding arch subdirectory. It must provide at least 3
files :
 - picoRTOS_types.h
 - picoRTOS_port.c
 - picoRTOS_portasm.S (or any asm extension required by your compiler)

The reason for this assembly file is not to mix C and assembly language, as it makes
the static analysis difficult if not impossible.

Don't use inline assembly, especially with parameters.
The ASM macro is tolerated if stubbed to allow error-free splint analysis.

## Step 2

Provide a demo for your board in the demo subdirectory. This will serve as a basic
test for your port. A blink test is usually more than enough.
If you provide support for atomic operations, try to use IPCs in your demo, too.

Your code should build with no errors and no warnings.

## Step 3

Add a splint target for your new arch to the root Makefile.
Your code must pass the static analysis with zero errors and zero warnings.

If you use any external libraries or SDKs, the calls must be stubbed and the defines
re-defined (wrapped in ifdefs) to allow splint to do its job properly.

## Step 4

Unless you spot a real bug or see a huge improvement to be made, don't touch to the
common code (picoRTOS.c, picoRTOS-SMP.c and ipc/, notably).
Any modification in these files will affect all ports, and you shouldn't make your
port work at the expense of others.

## Step 5

run ./uncrustify at the root of the project to ensure your code complies with
the coding standard.

On some systems, depending on your version, uncrustify will affect some common header
files like picoRTOS.h and ipc/picoRTOS_cond.h. Please don't commit them.

## Step 6

Commit on your fork (in a dedicated branch) and make a pull request.

## Final word

I understand these guidelines might seem extremely severe, potentially starving this
project from talents, but they are here to garantee error-free software.

This is extremely important in safety critical applications for which this RTOS
has been designed in the first place.
