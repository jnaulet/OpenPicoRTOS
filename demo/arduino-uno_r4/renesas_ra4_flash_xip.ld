/*
 * SAM3X8E memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x00004000, LENGTH = 240k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 32k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
