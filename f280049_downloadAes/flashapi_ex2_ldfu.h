//###########################################################################
//
// FILE:    flashapi_ex2_LDFU.h
//
// TITLE:   Live DFU Header
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

#ifndef __FLASHAPI_EX2_LDFU_H__
#define __FLASHAPI_EX2_LDFU_H__

//
// Includes
//
#include "F021_F28004x_C28x.h"
#include "flashapi_ex2_flash_programming_c28.h" 
#include "driverlib.h"
#include "flashapi_ex2_commands.h"
#include "assert.h"
#include "device.h"
#include "string.h"

//
// Defines
//
typedef uint16_t (*uint16fptr)(uint16_t *);

typedef struct HEADER
{
    uint32_t BlockSize;
    uint32_t DestAddr;
}tpHEADER;
//
//load address
//
#define LOAD_ADDRESS            0x00B000


#define UB_ADDRES       0x080000
#define UB_ADDRES_END   0x080FFF

#define UL_ADDRES       0x081010
#define UL_ADDRES_END   0x083FFF

#define DSP_ADDRES      0x084000
#define DSP_ADDRES_END  0x08BFFF

#define CONFIG_ADDRES   0x08F000
#define DEFULAT_ADDRES  0x08C000

//
// WORDS_IN_FLASH_BUFFER: Specifies the size of the buffer used to write the 
//
//
#define  WORDS_IN_FLASH_BUFFER    0x04

//
// Do not change BUFFER_SIZE; BUFFER_SIZE is the size of a buffer
// that stores the bytes of a program received through SCI
//
#define BUFFER_SIZE               0x80

//
// Function Prototypes
//
uint32_t Write_Flash(uint32_t u32StartAddress,uint16_t *pu16DataBuffer,uint16_t  u16DataBufferSizeInWords);
int16_t Erase_FlashSector(uint32_t u32StartAddress,uint32_t u32endAddress);
int16_t ldfuLoad(uint32_t address);
int16_t ldfuCopyData(uint32_t address);



void fmstatFail(void);
uint32_t getLongData(uint16_t *);


extern uint16_t sciaGetWordData(uint16_t *p);
extern void initFlashSectors(void);
extern void exampleError(Fapi_StatusType status);
extern void UARTprintf(const char *pcString, ...);
extern uint16_t *getBlockHeader(tpHEADER *pHeader);

#endif // __FLASHAPI_EX2_LDFU_H__

//
// End of file
//
