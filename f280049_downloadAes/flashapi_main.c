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

#include <string.h>
#include <stdint.h>
#include "device.h"
#include "flashapi_ex2_ldfu.h"

#include ".//user/cmd.h"
#include ".//user/uartStdio.h"
#include ".//xmodem/xmodem.h"

#include <aes_encrypt/aes.h>

//
// Defines
//
#define SCI_BOOT                0x01    //GPIO28; //GPIO29 (CCARD)

#define LOADRamfuncsLoadStart  (uint32_t)(0x000081020)
#define LOADencryptSize    (uint32_t)(0x00000fE0)

#define DSPRamfuncsLoadStart   (uint32_t)(0x000088010)
#define encryptSize    (uint32_t)(0x00000FF0)

//
// Globals
//


#pragma DATA_SECTION(encryptCode, ".encryptCode");
#pragma DATA_ALIGN(encryptCode, 8);
uint16_t encryptCode[encryptSize+0x10];
#pragma DATA_SECTION(dencryptCode, ".encryptCode");
#pragma DATA_ALIGN(dencryptCode, 8);
uint16_t dencryptCode[encryptSize+0x10];

uint16_t *pCode=( uint16_t*)LOAD_ADDRESS;
uint16_t *pdata ;
int32_t bytecnts;

void exampleError(Fapi_StatusType status);
void initFlashSectors(void);
uint16_t sciaGetWordData(uint16_t *p);
uint16_t *getBlockHeader(tpHEADER *pHeader);

uint16_t sciaGetWordData(uint16_t *p)
{
    return *p;
}

void main(void)
{
    int16_t c_n;
    DCSM_CSMPasswordKey sCMDKey;
    uint32_t uidadd=0,i;
    uint16_t *pEncrydata;
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

    if(DCSM_getZone1CSMSecurityStatus()!=DCSM_STATUS_UNSECURE)
    {

        while(1)
        {
            UARTprintf ("\r\nplease put hexValue(1248aCEffff) for csmkey0 \r\n");
            sCMDKey.csmKey0 = UARTGetHexNum();//0xaa55aa55;
            UARTprintf("\r\ncsmkey0=%x\r\n",sCMDKey.csmKey0);

            UARTprintf ("please put hexValue csmkey1\r\n");
            sCMDKey.csmKey1 = UARTGetHexNum();//0x3F7FFFFF;
            UARTprintf("\r\ncsmkey1=%x\r\n",sCMDKey.csmKey1);

            UARTprintf ("please put hexValue csmkey2\r\n");
            sCMDKey.csmKey2 = UARTGetHexNum();//0xFFFFFFFF;
            UARTprintf("\r\ncsmkey2=%x\r\n",sCMDKey.csmKey2);

            UARTprintf ("please put hexValue csmkey3\r\n");
            sCMDKey.csmKey3 = UARTGetHexNum();//0xFFFFFFFF;
            UARTprintf("\r\ncsmkey3=%x\r\n",sCMDKey.csmKey3);

            UARTprintf ("csmkeys is correct? Y(y) or N(n)\r\n");
            if((UARTGetc() =='Y') || (UARTGetc() =='y')) break;
        }

        DCSM_unlockZone1CSM(&sCMDKey);

        if(DCSM_getZone1CSMSecurityStatus()!=DCSM_STATUS_UNSECURE)
        {
            UARTprintf("\r\n ...key error...\r\n");
        }
    }

    initFlashSectors();
    gethelp();

   for(;;)
   {
       c_n=getCMDlines();
       switch(c_n)
       {
           case UP_DSP:
               bytecnts = xmodemReceive(pCode, 0x009000);//最大32Kwords
               pdata = pCode;
               if (bytecnts < 0)
                 {
                     UARTprintf ("\r\nXmodem receive error: status: %d\r\n", bytecnts);
                     UARTprintf ("receive words data nums > 0x009000!\r\n");
                     UARTprintf ("up data fail!\r\n");
                     break;

                 }
                 else
                 {
                     UARTprintf ("\r\nXmodem successfully received %d bytes\r\n", bytecnts*2);
                 }
                 if(ldfuLoad((uint32_t)DSP_ADDRES)>0)
                 {


                     memset(encryptCode,0,encryptSize*sizeof(uint16_t));

                     uidadd = 67321464;

                     SetCodeStatus(uidadd,(uint16_t *)DSPRamfuncsLoadStart,encryptCode);
                     pEncrydata = encryptCode+0x10;
                     encrypt_data(uidadd,(uint16_t *)DSPRamfuncsLoadStart,pEncrydata,encryptSize);

                     decrypt_data(uidadd,pEncrydata,dencryptCode,encryptSize);
                     pEncrydata = (uint16_t *)DSPRamfuncsLoadStart;
                     for(i=0;i<encryptSize;i++)
                     {
                         if(pEncrydata[i]!=dencryptCode[i])
                         {
                             uidadd=1;
                             UARTprintf ("S...:decrypt_data!=encrypt_data i=%d\r\n",i);
                             UARTprintf ("up data fail!\r\n");
                             break;
                         }
                     }
                     if(uidadd!=1)
                     {
                         if(0==Erase_FlashSector(DSPRamfuncsLoadStart,DSPRamfuncsLoadStart+encryptSize-1))
                         {
                             if(Write_Flash(DSPRamfuncsLoadStart-0x10,encryptCode,sizeof(encryptCode))>0)
                             UARTprintf ("S...:UP_DSP up data success!\r\n");

                         }
                     }
                 }
                 else
                 {
                     UARTprintf ("UP_DSP up data fail!\r\n");
                 }
               break;
           case UP_LOAD:
               bytecnts = xmodemReceive(pCode, 0x009000);//最大32Kwords
               pdata = pCode;
               if (bytecnts < 0)
                 {
                     UARTprintf ("\r\nXmodem receive error: status: %d\r\n", bytecnts);
                     UARTprintf ("receive words data nums > 0x009000!\r\n");
                     UARTprintf ("up data fail!\r\n");
                     break;

                 }
                 else
                 {
                     UARTprintf ("\r\nXmodem successfully received %d bytes\r\n", bytecnts*2);
                 }
                 if(ldfuLoad((uint32_t)UL_ADDRES)>0)
                 {


                     memset(encryptCode,0,encryptSize*sizeof(uint16_t));
                     uidadd = 67321464;
                     SetCodeStatus(uidadd,(uint16_t *)LOADRamfuncsLoadStart,encryptCode);
                     memcpy(encryptCode+0x10, (void *)(LOADRamfuncsLoadStart-0x10), 0x10);
                     pEncrydata = encryptCode+0x20;
                     encrypt_data(uidadd,(uint16_t *)LOADRamfuncsLoadStart,pEncrydata,LOADencryptSize);

                     decrypt_data(uidadd,pEncrydata,dencryptCode,LOADencryptSize);
                     pEncrydata = (uint16_t *)LOADRamfuncsLoadStart;
                     for(i=0;i<LOADencryptSize;i++)
                     {
                         if(pEncrydata[i]!=dencryptCode[i])
                         {
                             uidadd=1;
                             UARTprintf ("S...:decrypt_data!=encrypt_data i=%d\r\n",i);
                             UARTprintf ("UP_LOAD fail!\r\n");
                             break;
                         }
                     }
                     if(uidadd!=1)
                     {
                         if(0==Erase_FlashSector(LOADRamfuncsLoadStart,LOADRamfuncsLoadStart+LOADencryptSize-1))
                         {
                             if(Write_Flash(LOADRamfuncsLoadStart-0x20,encryptCode,sizeof(encryptCode))>0)
                             UARTprintf ("S...:UP_LOAD up data success!\r\n");
                         }
                     }

                 }
                 else
                 {
                     UARTprintf ("UP_LOAD up data fail!\r\n");
                 }
               break;
           case UP_UBOOT:
               bytecnts = xmodemReceive(pCode, 0x009000);//最大32Kwords
               pdata = pCode;
               if (bytecnts < 0)
                 {
                     UARTprintf ("\r\nXmodem receive error: status: %d\r\n", bytecnts);
                     UARTprintf ("receive words data nums > 0x009000!\r\n");
                     UARTprintf ("up data fail!\r\n");
                     break;
                 }
                 else
                 {
                     UARTprintf ("\r\nXmodem successfully received %d bytes\r\n", bytecnts);
                 }
                 if(ldfuLoad((uint32_t)UB_ADDRES)>0)
                 {
                     UARTprintf ("UP_UBOOT up data success!\r\n");
                 }
                 else
                 {
                     UARTprintf ("UP_UBOOT up data fail!\r\n");

                 }
               break;
           case REBOOT:
               //Flash_powerDown(FLASH0CTRL_BASE);
               SysCtl_setWatchdogMode(SYSCTL_WD_MODE_RESET);

               //
               // Enable the watchdog
               //
               SysCtl_enableWatchdog();
           default:
               UARTprintf("This is bad cmd\r\n");
               gethelp();
               break;
       }


   }


}




// initFlashSectors - Initializes the flash API
//
void initFlashSectors(void)
{
    //
    // For the Live DFU configurations, error correction is not disabled when
    // initializing the flash API
    //
    EALLOW;

    Fapi_StatusType oReturnCheck;
    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, 100);
    if(oReturnCheck != Fapi_Status_Success)
    {
        exampleError(oReturnCheck);
    }

    oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);

    if(oReturnCheck != Fapi_Status_Success)
    {
        exampleError(oReturnCheck);
    }

    EDIS;
    
}

//
// Example_Error - Error function that will halt debugger
//
void exampleError(Fapi_StatusType status)
{
    //
    // Error code will be in the status parameter
    //
    UARTprintf ("initFlashSectors:exampleError\r\n");
    __asm("    ESTOP0");
}



uint16_t *getBlockHeader(tpHEADER *pHeader)
{
    tpHEADER tempHader;
    uint16_t *pTempCode = NULL;
    uint16_t * retadd=NULL;
    uint32_t add;

    retadd=NULL;
    pTempCode = pCode+11;
    tempHader.BlockSize = *pTempCode++;
    pHeader->DestAddr=0xFFFFFFFF;
    while(tempHader.BlockSize>0)
    {
        add = ((uint32_t)(*pTempCode++))<<16;
        tempHader.DestAddr = add|*pTempCode++;
        if(pHeader->DestAddr>tempHader.DestAddr)
        {
            pHeader->DestAddr = tempHader.DestAddr;
            pHeader->BlockSize = tempHader.BlockSize;
            retadd = pTempCode;
        }
        pTempCode += tempHader.BlockSize;
        tempHader.BlockSize = *pTempCode++;
    }
    if(retadd!=NULL)
    {
        retadd[-1]=0xffff;
        retadd[-2]=0xffff;
    }
    return retadd;
}

//
// End of file
//
