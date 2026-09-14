/*
 * Memory map for MSP430G2553
 */
MEMORY
{
    ROM(rx)      : ORIGIN = 0xc000, LENGTH = 16320
    VECTORS(rx)  : ORIGIN = 0xffc0, LENGTH = 64
    RAM(rw)      : ORIGIN = 0x0200, LENGTH = 512
}

ENTRY(Reset_Handler)
