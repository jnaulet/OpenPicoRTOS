/*
 * GD32VF103CBT6 memory mapping for ROM
 */

MEMORY
{
    /* boot(rx)    : ORIGIN = 0x1fffb000, LENGTH = 18k */
    flash(rx)   : ORIGIN = 0x08000000, LENGTH = 128k
    ram(rwx)    : ORIGIN = 0x20000000, LENGTH = 32k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);
