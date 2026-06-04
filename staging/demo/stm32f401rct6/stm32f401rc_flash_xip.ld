/*
 * STM32F401RC memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x08000000, LENGTH = 256k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 64k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
