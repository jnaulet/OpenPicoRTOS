#ifndef CLOCK_ATTINY88_H
#define CLOCK_ATTINY88_H

#include "clock.h"

#define CLOCK_ATTINY88_CLKCPU   CLOCK_SYSCLK
#define CLOCK_ATTINY88_CLKIO    (clock_id_t)1
#define CLOCK_ATTINY88_CLKFLASH (clock_id_t)2
#define CLOCK_ATTINY88_CLKADC   (clock_id_t)3
#define CLOCK_ATTINY88_CLKTWIHS (clock_id_t)4
#define CLOCK_ATTINY88_COUNT    (clock_id_t)5

#endif
