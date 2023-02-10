# Release notes

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
