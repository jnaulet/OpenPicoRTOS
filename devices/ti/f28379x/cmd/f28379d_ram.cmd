
MEMORY
{
   BEGIN            : origin = 0x080000, length = 0x000400
   BOOT_RSVD        : origin = 0x000010, length = 0x000199
   /* MRAM */
   RAMM0            : origin = 0x0001A9, length = 0x000257
   RAMM1            : origin = 0x000400, length = 0x0003F8
   /* DRAM */
   RAMD0            : origin = 0x00C000, length = 0x000800
   RAMD1            : origin = 0x00C800, length = 0x000800
   /* LSRAM */
   RAMLS0_7         : origin = 0x008000, length = 0x004000
   /* On-chip flash */
   FLASH0_13        : origin = 0x080400, length = 0x03FBFF
   /* Reset */
   RESET            : origin = 0x3FFFC0, length = 0x000002
}

SECTIONS
{
   .init            : > BEGIN, ALIGN(8)
   .text            : { *.lib(.text) } > FLASH0_13, ALIGN(8)
   .cinit           : > FLASH0_13, ALIGN(8)
   .switch          : > FLASH0_13, ALIGN(8)
   .reset           : > RESET, TYPE = DSECT
   .stack           : > RAMM1

#if defined(__TI_EABI__)
   .init_array      : > FLASH0_13, ALIGN(8)
   .bss             : > RAMLS0_7
   .bss:output      : > RAMLS0_7
   .bss:cio         : > RAMLS0_7
   .data            : > RAMLS0_7
   .sysmem          : > RAMM1
   .const           : > FLASH0_13, ALIGN(8)
#else
   .pinit           : > FLASH0_13, ALIGN(8)
   .ebss            : > RAMLS0_7
   .esysmem         : > RAMM1
   .cio             : > RAMLS0_7
   .econst          : >> FLASH0_13, ALIGN(8)
#endif

#if defined(__TI_EABI__)
   .TI.ramfuncs     : { *(.text) } LOAD = FLASH0_13,
                        RUN = RAMLS0_7
                        LOAD_START(RamfuncsLoadStart),
                        LOAD_SIZE(RamfuncsLoadSize),
                        RUN_START(RamfuncsRunStart),
                        RUN_END(RamfuncsRunEnd),
                        ALIGN(8)
#else
   .TI.ramfuncs     : { *(.text) } LOAD = FLASH0_13,
                        RUN = RAMLS0_7,
                        LOAD_START(_RamfuncsLoadStart),
                        LOAD_SIZE(_RamfuncsLoadSize),
                        RUN_START(_RamfuncsRunStart),
                        RUN_END(_RamfuncsRunEnd),
                        ALIGN(8)
#endif
}
