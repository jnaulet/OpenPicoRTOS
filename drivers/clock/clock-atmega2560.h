#ifndef CLOCK_ATMEGA2560_H
#define CLOCK_ATMEGA2560_H

#include "clock.h"

#define CLOCK_ATMEGA2560_CLKCPU   CLOCK_SYSCLK
#define CLOCK_ATMEGA2560_CLKFLASH (clock_id_t)1
#define CLOCK_ATMEGA2560_CLKADC   (clock_id_t)2
#define CLOCK_ATMEGA2560_CLKIO    (clock_id_t)3
#define CLOCK_ATMEGA2560_CLKASY   (clock_id_t)4
#define CLOCK_ATMEGA2560_CLKPTC   (clock_id_t)5
#define CLOCK_ATMEGA2560_CLKSYS   (clock_id_t)6
#define CLOCK_ATMEGA2560_COUNT    (clock_id_t)7

#endif
