
MEMORY
{
   BEGIN            : origin = 0x080000, length = 0x000400
   BOOT_RSVD        : origin = 0x000010, length = 0x000199
   RAMM0_1          : origin = 0x0001a9, length = 0x000657
   RAMLS0_5         : origin = 0x008000, length = 0x003000
   RAMD0_1          : origin = 0x00b000, length = 0x001000
   RAMGS0_15        : origin = 0x00c000, length = 0x010000
   FLASH0_13        : origin = 0x080400, length = 0x03fbff
   RESET            : origin = 0x3fffc0, length = 0x000002
}

SECTIONS
{
   .init            : > BEGIN, ALIGN(8)
   .text            : > FLASH0_13, ALIGN(8)
   .cinit           : > FLASH0_13, ALIGN(8)
   .switch          : > FLASH0_13, ALIGN(8)
   .reset           : > RESET, TYPE = DSECT
   .stack           : > RAMD0_1

#if defined(__TI_EABI__)
   .init_array      : > FLASH0_13, ALIGN(8)
   .bss             : > RAMGS0_15
   .bss:output      : > RAMGS0_15
   .bss:cio         : > RAMGS0_15
   .data            : > RAMGS0_15
   .sysmem          : > RAMM0_1
   .const           : > FLASH0_13, ALIGN(8)
#else
   .pinit           : > FLASH0_13, ALIGN(8)
   .ebss            : > RAMGS0_15
   .esysmem         : > RAMM0_1
   .cio             : > RAMGS0_15
   .econst          : > FLASH0_13, ALIGN(8)
#endif

#if defined(__TI_EABI__)
   .TI.ramfuncs     : { *(.ramfunc) *(.ramfuncs) } LOAD = FLASH0_13,
                        RUN = RAMLS0_5
                        LOAD_START(RamfuncsLoadStart),
                        LOAD_SIZE(RamfuncsLoadSize),
                        RUN_START(RamfuncsRunStart),
                        RUN_END(RamfuncsRunEnd),
                        ALIGN(8)
#else
   .TI.ramfuncs     : { *(.ramfunc) *(.ramfuncs) } LOAD = FLASH0_13,
                        RUN = RAMLS0_5,
                        LOAD_START(_RamfuncsLoadStart),
                        LOAD_SIZE(_RamfuncsLoadSize),
                        RUN_START(_RamfuncsRunStart),
                        RUN_END(_RamfuncsRunEnd),
                        ALIGN(8)
#endif
}
