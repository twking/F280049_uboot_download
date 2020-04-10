


-stack          0x0000300 /* Stack Size */

#define _FLASH


MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */
#if defined(_FLASH)
   BEGIN           : origin = 0x081010, length = 0x000002//BEGIN           : origin = 0x080000, length = 0x000002
#else
   BEGIN           : origin = 0x000000, length = 0x000002
#endif

   RAMM0           	: origin = 0x0000F5, length = 0x00030B
   RAMLS0_5         : origin = 0x008000, length = 0x003000
   RESET           	: origin = 0x3FFFC0, length = 0x000002

 /* Flash sectors: you can use FLASH for program memory when the RAM is filled up*/
   /* BANK 0 */
   FLASH0_BOOT_S0_S1  : origin = 0x080000, length = 0x001000	/* on-chip Flash */
   FLASH0_LOAD_S1_S3  : origin = 0x081020, length = 0x002fE0	/* on-chip Flash */
   FLASH0_DSP_S4_S7   : origin = 0x084002, length = 0x003ffc	/* on-chip Flash */
   FLASH0_DSP_S8_S9   : origin = 0x088000, length = 0x003000	/* on-chip Flash */
   FLASH0_CLA_S10     : origin = 0x08B000, length = 0x001000	/* on-chip Flash */
   FLASH0_S11_S14     : origin = 0x08C000, length = 0x003000	/* on-chip Flash */
   FLASH0_CONFIG_S15  : origin = 0x08F000, length = 0x001000	/* on-chip Flash */

  /* BANK 1 */
   FLASH1_RES_S0_15  : origin = 0x090000, length = 0x010000	/* on-chip Flash */

PAGE 1 :

   BOOT_RSVD       : origin = 0x000002, length = 0x0000F3     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
   uploadram   	   : origin = 0x00B000, length = 0x008000
   EBSS   	   	   : origin = 0x00B000+0x008000, length = 0x001000
}

/*You can arrange the .text, .cinit, .const, .pinit, .switch and .econst to FLASH when RAM is filled up.*/
SECTIONS
{

    codestart    : > BEGIN,     PAGE = 0


#if defined(_FLASH)
   GROUP
   {
       .text
       .TI.ramfunc
       .pinit
       .cinit
       .switch
       .econst
   } LOAD = FLASH0_LOAD_S1_S3,
     RUN = RAMLS0_5,
     LOAD_START(_RamfuncsLoadStart),
     LOAD_SIZE(_RamfuncsLoadSize),
     LOAD_END(_RamfuncsLoadEnd),
     RUN_START(_RamfuncsRunStart),
     RUN_SIZE(_RamfuncsRunSize),
     RUN_END(_RamfuncsRunEnd),
     PAGE = 0, ALIGN(4)

#else
 	.TI.ramfunc     : > RAMLS0_5,     PAGE = 0,ALIGN(4)
 	LDFU_BANK1      : > RAMLS0_5,  	  PAGE = 0,ALIGN(4)

   .text            : > RAMLS0_5,  	  PAGE = 0,ALIGN(4)
   .pinit           : > RAMLS0_5,     PAGE = 0,ALIGN(4)
   .cinit           : > RAMLS0_5,     PAGE = 0,ALIGN(4)
   .econst          : > RAMLS0_5,     PAGE = 0,ALIGN(4)

   .switch          : > RAMLS0_5,     PAGE = 0,ALIGN(4)


#endif

   .reset           : > RESET,     		PAGE = 0, TYPE = DSECT /* not used, */
   .stack           : > RAMM0,     		PAGE = 0,ALIGN(4)
   .ebss            : > EBSS,   		PAGE = 1,ALIGN(4)
   .esysmem         : > RAMM1,     		PAGE = 1,ALIGN(4)
   .cio             : > RAMM1,     		PAGE = 1,ALIGN(4)
   .encryptCode     : > uploadram		PAGE = 1,ALIGN(8)

}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
