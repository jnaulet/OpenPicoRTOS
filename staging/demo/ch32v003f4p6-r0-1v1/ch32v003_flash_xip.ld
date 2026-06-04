/*
 * CH32V003 memory mapping for ROM
 */

MEMORY
{
    /* boot(rx)    : ORIGIN = 0x1ffff000, LENGTH = 1920 */
    rom(rx)     : ORIGIN = 0x00000000, LENGTH = 16k
    ram(rwx)    : ORIGIN = 0x20000000, LENGTH = 2k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", rom);
