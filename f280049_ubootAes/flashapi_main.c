//###########################################################################
//
// FILE:   flashapi_ex2_sciKernel.c
//
// TITLE: SCI Flash Kernel Example for F28004x
//
//! \addtogroup cpu01_example_list
//! <h1>SCI Flash Kernel Example for F28004x</h1>
//!
//! In this example, we set up a UART connection with a host using SCI, receive
//! commands for CPU1 to perform which then sends ACK, NAK, and status packets
//! back to the host after receiving and completing the tasks. Each command
//! either expects no data from the command packet or specific data relative
//! to the command.
//!
//! The example has seven build configurations, listed below.
//!         BANK0_LDFU: Only supports the Live Device Firmware Update (LDFU)
//!                     command for Bank 0; other commands are not supported
//!                     by this build configuration. For more information about
//!                     the LDFU command functionality, please read the documentation 
//!                     in 'flashapi_ex2_LDFU.c'. Links the flash kernel to Bank 0
//!                     (0x80000 - 0x82007 will be reserved). Uses Flash API symbols 
//!                     in flash.
//!                     
//!         BANK0_LDFU_ROM: Only supports the Live Device Firmware Update (LDFU)
//!                         command for Bank 0; other commands are not supported
//!                         by this build configuration. For more information about
//!                         the LDFU command functionality, please read the documentation 
//!                         in 'flashapi_ex2_LDFU.c'. Links the flash kernel to Bank 0
//!                         (0x80000 - 0x82007 will be reserved). Uses Flash API symbols 
//!                         in ROM; Rev A of F28004x cannot be used with this build  
//!                         configuration.
//!                         
//!         BANK0_NO_LDFU: Supports all commands except the LDFU command. Links the
//!                        flash kernel to Bank 0. Uses Flash API symbols in flash.
//!                        Commands must only be used to manipulate flash in Bank 1.
//!         
//!         BANK1_LDFU: Only supports the Live Device Firmware Update (LDFU)
//!                     command for Bank 1; other commands are not supported
//!                     by this build configuration. For more information about
//!                     the LDFU command functionality, please read the documentation 
//!                     in 'flashapi_ex2_LDFU.c'. Links the flash kernel to Bank 1
//!                     (0x90000 - 0x92007 will be reserved).Uses Flash API symbols in   
//!                     flash.
//!
//!         BANK1_LDFU_ROM: Only supports the Live Device Firmware Update (LDFU)
//!                         command for Bank 1; other commands are not supported
//!                         by this build configuration. For more information about
//!                         the command functionality, please read the documentation 
//!                         in 'flashapi_ex2_LDFU.c'. Links the flash kernel to Bank 1.
//!                         (0x90000 - 0x92007 will be reserved) Uses Flash API  
//!                         symbols in ROM; Rev A of F28004x cannot be used with this 
//!                         build configuration.
//!                         
//!         BANK1_NO_LDFU: Supports all commands except the LDFU command. Links the
//!                        flash kernel to Bank 1.  Uses Flash API symbols in flash.
//!                        Commands must only be used to manipulate flash in Bank 0.
//!
//!         CPU1_RAM: Supports all commands except the LDFU command. Links the flash
//!                   kernel to RAM.
//!
//! This example is to be used with the Serial Flash Programmer. Applications loaded to
//! flash through commands of the Serial Flash Programmer must be in the '--boot --sci8
//! --ascii' hex format, which can be achieved by using the hex utility tool.
//!
//! \b External \b Connections \n
//!  - Connect SCI Rx and SCI Tx to a COM port of the computer
//!    running the Serial Flash Programmer project
//!
//! \b Watch \b Variables \n
//!  - None
//!          
//
//###########################################################################
// $TI Release: F28004x Support Library v1.06.00.00 $
// $Release Date: Mon May 27 06:42:25 CDT 2019 $
// $Copyright:
// Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Includes
//

//#include <string.h>

#include <aes_decrypt/aes.h>
#include "device.h"


//
// Function Prototypes
//


#define pLOADRamfuncsLoadStart  (uint16_t *)(0x00081020)
#define sLOADRamfuncsRunSize    0x00002FE0
#define pLOADRamfuncsRunStart   (uint16_t *)(0x00008000)

#define pDSPRamfuncsLoadStart  (uint16_t *)(0x000088010)
#define sDSPRamfuncsRunSize    0x00002FF0
#define pDSPRamfuncsRunStart   (uint16_t *)(0x000008800)

#define pCLARamfuncsLoadStart  (uint16_t *)(0x0008B000)
#define sCLARamfuncsRunSize    0x00000800
#define pCLARamfuncsRunStart   (uint16_t *)(0x000008000)

#define encryptSize    (0x00001000)

#pragma DATA_SECTION(encryptCode, ".encryptCode");
#pragma DATA_ALIGN(encryptCode, 8);
uint16_t encryptCode[encryptSize];


#ifndef __cplusplus
#pragma CODE_SECTION(main1, ".TI.ramfunc");
#endif
#ifdef __cplusplus
#pragma CODE_SECTION(".TI.ramfunc");
#endif
void main1(void)
{
    int16_t i,n=0;
    int32_t delay_nums,t=0;
    uint32_t uidadd=0;
    SCI_writeCharBlockingNonFIFO(SCIA_BASE, '0');
    SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'U');
    while(1)
      {
         for(i=0;i<128;i++)
         {
             delay_nums = 0;
             t =-1;
             while(0xe5!=t)
             {
                 t =SCI_readCharNonBlocking(SCIA_BASE);
                 delay_nums++;
                 if(delay_nums>1600)//>=80
                 {
                     n++;
                     break;
                 }
             }
         }
         if(n>=120)//加载正常运行程序
         {
             memcpy(pCLARamfuncsRunStart, pCLARamfuncsLoadStart, sCLARamfuncsRunSize);

             uidadd = 67321464;

             if(getCodeStatus(uidadd,(uint16_t *)pDSPRamfuncsLoadStart))
             {
                 memcpy(pDSPRamfuncsRunStart, pDSPRamfuncsLoadStart, sDSPRamfuncsRunSize);
             }
             else
             {
                 SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'S');

                 memcpy(encryptCode, pDSPRamfuncsLoadStart, encryptSize-0x10);

                 decrypt_data(uidadd,encryptCode,(uint16_t *)pDSPRamfuncsRunStart,encryptSize-0x10);

                 memcpy(pDSPRamfuncsRunStart+encryptSize-0x10, pDSPRamfuncsLoadStart+encryptSize-0x10, sDSPRamfuncsRunSize-encryptSize+0x10);
             }
             SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'P');
             DEVICE_DELAY_US(1000);
             asm("    LB 0x084000");
         }
         else//加载flash烧入程序
         {
             uidadd = 67321464;

             if(getCodeStatus(uidadd,(uint16_t *)pLOADRamfuncsLoadStart))
             {
                 memcpy(pLOADRamfuncsRunStart,pLOADRamfuncsLoadStart, sLOADRamfuncsRunSize);
             }
             else
             {
                 SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'S');

                 memcpy(encryptCode, pLOADRamfuncsLoadStart, encryptSize-0x20);

                 decrypt_data(uidadd,encryptCode,(uint16_t *)pLOADRamfuncsRunStart,encryptSize-0x20);

                 memcpy(pLOADRamfuncsRunStart+encryptSize-0x20, pLOADRamfuncsLoadStart+encryptSize-0x20, sLOADRamfuncsRunSize-encryptSize+0x20);
             }
             SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'L');

             DEVICE_DELAY_US(1000);

             asm("    LB 0x081010");
         }
      }

}
void main(void)
{

   Device_init();
   Device_initGPIO();
   Device_intSCIA();
   //
   //
   // init interrupt and vectorTable, drivelib.
   //
   Interrupt_initModule();
   Interrupt_initVectorTable();

   //
   // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
   //

   EINT;
   ERTM;
   main1();

}

