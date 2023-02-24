#include "misc-pic32mx.h"
#include "picoRTOS.h"
#include "picoRTOS_device.h"

#define CONFIG_CFGCON ((volatile uint32_t*)(ADDR_CONFIGURATION + 0x0))
#define CONFIG_SYSKEY ((volatile uint32_t*)(ADDR_CONFIGURATION + 0x30))

/* Function: pic32mx_unlock
 * Unlocks system protected registers
 */
void pic32mx_unlock(void)
{
#define KEY1 0xaa996655
#define KEY2 0x556699aa

    *CONFIG_SYSKEY = (uint32_t)KEY1;
    *CONFIG_SYSKEY = (uint32_t)KEY2;
}

/* Function: pic32mx_lock
 * Locks system protected registers
 */
void pic32mx_lock(void)
{
#define KEY3 0x33333333

    *CONFIG_SYSKEY = (uint32_t)KEY3;
}
