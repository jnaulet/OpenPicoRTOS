#ifndef CLOCK_ATMEGA328P_H
#define CLOCK_ATMEGA328P_H

#include "clock.h"

#define CLOCK_ATMEGA328P_CLKCPU   CLOCK_SYSCLK
#define CLOCK_ATMEGA328P_CLKFLASH (clock_id_t)1
#define CLOCK_ATMEGA328P_CLKADC   (clock_id_t)2
#define CLOCK_ATMEGA328P_CLKIO    (clock_id_t)3
#define CLOCK_ATMEGA328P_CLKASY   (clock_id_t)4
#define CLOCK_ATMEGA328P_CLKPTC   (clock_id_t)5
#define CLOCK_ATMEGA328P_CLKSYS   (clock_id_t)6
#define CLOCK_ATMEGA328P_COUNT    (clock_id_t)7

#endif
