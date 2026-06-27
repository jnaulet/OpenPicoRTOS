/*
 * STM342F103xx memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x08000000, LENGTH = 64k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 20k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
