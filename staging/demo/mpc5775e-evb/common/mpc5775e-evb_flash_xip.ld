/*
 * MPC5775E memory mapping for ROM
 */

MEMORY
{
    flash(rx) : ORIGIN = 0x00800000, LENGTH = 4096k
    ram(rwx)  : ORIGIN = 0x40000000, LENGTH = 512k
}

ENTRY(Reset_Handler)
REGION_ALIAS("REGION_TEXT", flash);

/* ECC RAM */
PROVIDE(__ram_start__ = ORIGIN(ram));
PROVIDE(__ram_end__ = ORIGIN(ram) + LENGTH(ram) - 1);
