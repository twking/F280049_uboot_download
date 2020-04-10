


-stack          0x0000300 /* Stack Size */
-heap			0x0001000 /* Stack Size */

#define _FLASH


MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */
#if defined(_FLASH)
   BEGIN           : origin = 0x080000, length = 0x000002//BEGIN           : origin = 0x080000, length = 0x000002
#else
   BEGIN           : origin = 0x000000, length = 0x000002
#endif

   RAMM0           	: origin = 0x0000F5, length = 0x00030B
   RAMLS0_6         : origin = 0x008000, length = 0x003800
   cla_dsp_LS7		: origin = 0x00B800, length = 0x000800
   RAMGS0_1   		: origin = 0x00C000, length = 0x004000
   RESET           	: origin = 0x3FFFC0, length = 0x000002

 /* Flash sectors: you can use FLASH for program memory when the RAM is filled up*/
   /* BANK 0 */
   FLASH0_BOOT_S0_S1  : origin = 0x080002, length = 0x000FFC	/* on-chip Flash */
   FLASH0_LOAD_S1_S3  : origin = 0x081000, length = 0x003000	/* on-chip Flash */
   FLASH0_DSP_S4_S7   : origin = 0x084000, length = 0x004000	/* on-chip Flash */
   FLASH0_DSP_S8_S9   : origin = 0x088000, length = 0x003000	/* on-chip Flash */
   FLASH0_CLA_S10     : origin = 0x08B000, length = 0x001000	/* on-chip Flash */
   FLASH0_S11_S14     : origin = 0x08C000, length = 0x003000	/* on-chip Flash */
   FLASH0_CONFIG_S15  : origin = 0x08F000, length = 0x001000	/* on-chip Flash */
  /* BANK 1 */
   FLASH1_RES_S0_15  : origin = 0x090000, length = 0x010000	/* on-chip Flash */

PAGE 1 :

   BOOT_RSVD       : origin = 0x000002, length = 0x0000F3     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */

   RAMGS2_3   	   : origin = 0x010000, length = 0x004000
}

/*You can arrange the .text, .cinit, .const, .pinit, .switch and .econst to FLASH when RAM is filled up.*/
SECTIONS
{

    codestart       : > BEGIN,     PAGE = 0


#if defined(_FLASH)

       .text 		: > FLASH0_BOOT_S0_S1,     PAGE = 0,ALIGN(4)
       .pinit		: > FLASH0_BOOT_S0_S1,     PAGE = 0,ALIGN(4)
       .cinit		: > FLASH0_BOOT_S0_S1,     PAGE = 0,ALIGN(4)
       .switch		: > FLASH0_BOOT_S0_S1,     PAGE = 0,ALIGN(4)
       //.econst		: > FLASH0_BOOT_S0_S1,     PAGE = 0,ALIGN(4)
	   GROUP
       {
        .TI.ramfunc
       	.econst
       } LOAD = FLASH0_BOOT_S0_S1,
          RUN = cla_dsp_LS7,
          LOAD_START(_RamfuncsLoadStart),
          LOAD_SIZE(_RamfuncsLoadSize),
          LOAD_END(_RamfuncsLoadEnd),
          RUN_START(_RamfuncsRunStart),
          RUN_SIZE(_RamfuncsRunSize),
          RUN_END(_RamfuncsRunEnd),
          PAGE = 0, ALIGN(4)

#else
 	.TI.ramfunc     : > RAMGS0_1,     PAGE = 0,ALIGN(4)
   .text            : > RAMGS0_1,     PAGE = 0,ALIGN(4)
   .pinit           : > RAMGS0_1,     PAGE = 0,ALIGN(4)
   .econst          : > RAMGS0_1,     PAGE = 0,ALIGN(4)
   .cinit           : > RAMGS0_1,     PAGE = 0,ALIGN(4)
   .switch          : > RAMGS0_1,     PAGE = 0,ALIGN(4)

#endif

   .reset           : > RESET,     	  PAGE = 0, TYPE = DSECT /* not used, */
   .stack           : > RAMM1,  	  PAGE = 1,ALIGN(4)
   .ebss            : > RAMGS2_3,     PAGE = 1,ALIGN(4)
   .esysmem         : > RAMGS2_3,     PAGE = 1,ALIGN(4)
   .cio             : > RAMGS2_3,     PAGE = 1,ALIGN(4)
   .encryptCode		: > RAMGS2_3,     PAGE = 1,ALIGN(4)
   .aes_test      	: > cla_dsp_LS7   PAGE = 0,ALIGN(4)
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
