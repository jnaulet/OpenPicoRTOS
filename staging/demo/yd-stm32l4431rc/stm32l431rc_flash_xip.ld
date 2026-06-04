/*
 * STM32F401RC memory mapping for ROM
 */

MEMORY
{
    flash(rx)    : ORIGIN = 0x08000000, LENGTH = 256k
    ram(rwx)     : ORIGIN = 0x20000000, LENGTH = 48k
    ram2(rwx)    : ORIGIN = 0x10000000, LENGTH = 16k /* with parity check */
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
