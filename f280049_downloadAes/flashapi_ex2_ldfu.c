//###########################################################################
//
// FILE:    flashapi_ex2_LDFU.c
//
// TITLE:   Live DFU Command Functionality
//
// This file contains the functionality of the Live Device Firmware Update
// (Live DFU or LDFU) Command and bank selection logic. The command functionality has
// 2 build configurations for each bank: BANK0_LDFU, BANK0_LDFU_ROM, BANK1_LDFU,
// and BANK1_LDFU_ROM.
//
// For the BANK0 build configurations, the following steps are taken when the
// kernel receives the Live DFU command:
//      1. Read an SCI Boot hex formatted file until information related to
//         each block of data remains
//      2. Read and store the revision values of bank 1 and bank 0 from flash
//         (B1_REV_ADD: 0x92006)
//      2. Erase sectors 2-15 of bank 1
//      3. Write 64 bits of 'START' value to 0x92000 (B1_START_ADD) to indicate
//         that erasing is done and programming/verifying is about to start
//      4. Program and verify bank 1 by receiving the SCI boot hex formatted
//         file one block of data at a time, writing each byte to flash, and verifying
//         each byte (the data should not be linked to an address that is less
//         than 0x92008 (B1_RESERVED))
//      5. Decrement the revision value of bank 1
//      6. Write the 'KEY' value to 0x92004 (B1_KEY_ADD) and the
//         revision value of bank 1 to 0x92006 (B1_REV_ADD)
//      7. Configure the watchdog for a reset and enable the watchdog in order
//         for a reset to occur
//
// For the BANK1 build configurations, the following steps are taken when the
// kernel receives the Live DFU command:
//      1. Read an SCI Boot hex formatted file until information related to
//         each block of data remains
//      2. Read and store the revision value of bank 0 and bank 1 from flash
//         (B0_REV_ADD: 0x82006)
//      2. Erase sectors 2-15 of bank 0
//      3. Write 64 bits of 'START' value to 0x82000 (B0_START_ADD) to indicate that
//         erasing is done and programming/verifying is about to start
//      4. Program and verify bank 0 by receiving the SCI boot hex formatted
//         file one block of data at a time, writing each byte to flash, and verifying
//         each byte (the data should not be linked to an address that is less
//         than 0x82008 (B0_RESERVED))
//      5. Decrement the revision value of bank 0
//      6. Write the 'KEY' value of bank 0 to 0x82004 (B0_KEY_ADD) and the
//         revision value of bank 0 to 0x82006 (B0_REV_ADD)
//      7. Configure the watchdog for a reset and enable the watchdog in order
//         for a reset to occur
//
// Bank selection logic (bankSelect) is the entry point for the BANK0 build
// configurations; it is also the first thing to run after a reset occurs.
// Bank selection logic branches to the most recently programmed bank or to
// the kernel setup when no banks have been programmed using the Live DFU
// command. When no banks have been programmed using the Live DFU command, a 
// program must be loaded to bank 1 by using the Live DFU command.
//
// Bank selection logic is located at 0x80000; therefore the device must be
// configured to boot to flash at 0x80000 for correct functionality.
//
// When running BANK0 configurations, a breakpoint may need to be placed 
// at the beginning of bankSelect if CCS debug tools are needed. The breakpoint 
// may be removed afterwards to prevent the program from stopping after each update.
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
#include "flashapi_ex2_ldfu.h"

//
// Globals
//

//
// BUFF: Data Buffer used for programming 'KEY', 'REV', and 'START' values
//
#define GET_SECTION_NUMS(add) ((((uint32_t)(add))-Bzero_Sector0_start)/(B_8KSector_u32length<<1))

uint16   BUFF[WORDS_IN_FLASH_BUFFER + 1];
uint32   *Buffer32 = (uint32 *)BUFF;

//
// getWordData is a pointer to the function that interfaces to the peripheral.
// Each loader assigns this pointer to it's particular getWordData function.
//
uint16fptr getWordData;


//
// secAddress contains the starting address of each sector in each bank
// Bank 0 (sectors 0-15) and Bank 1 (sectors 0-15)
// The array is used to erase multiple sectors within a bank
//
const uint32_t secAddress[32] = { Bzero_Sector0_start, Bzero_Sector1_start,
                                 Bzero_Sector2_start, Bzero_Sector3_start,
                                 Bzero_Sector4_start, Bzero_Sector5_start,
                                 Bzero_Sector6_start, Bzero_Sector7_start,
                                 Bzero_Sector8_start, Bzero_Sector9_start,
                                 Bzero_Sector10_start, Bzero_Sector11_start,
                                 Bzero_Sector12_start, Bzero_Sector13_start,
                                 Bzero_Sector14_start, Bzero_Sector15_start,
                                 Bone_Sector0_start, Bone_Sector1_start,
                                 Bone_Sector2_start, Bone_Sector3_start,
                                 Bone_Sector4_start, Bone_Sector5_start,
                                 Bone_Sector6_start, Bone_Sector7_start,
                                 Bone_Sector8_start, Bone_Sector9_start,
                                 Bone_Sector10_start, Bone_Sector11_start,
                                 Bone_Sector12_start, Bone_Sector13_start,
                                 Bone_Sector14_start, Bone_Sector15_start };

//
// Typedefs
//

//
// fmstatFail -  fmstatFail is the function that is executed when an error
//               occurs in the Flash State Machine (FSM). By default, it
//               halts execution, so that the error can be observed.
//
void fmstatFail(void)
{
    //
    //  Error code will be in the status parameter
    //
    UARTprintf("flash error :fmstatFail\n\r");

}

//
// ldfuLoad - Reads in an SCI boot hex formatted file until information related to
//            each block of data remains. Calls on ldfuCopyData in order to
//            erase flash, receive the remaining data via SCI, program the data
//            to flash, and verify the data in flash; also returns the starting address
//            specified by the file
//
int16_t ldfuLoad(uint32_t address)
{
    //
    // EntryAddr is used to store the starting address soecified by the file
    // being read and loaded to flash
    //
    uint32_t EntryAddr;
    //
    // Assign getWordData to the SCI-A version of the
    // function. getWordData is a pointer to a function.
    //
    getWordData = sciaGetWordData;

    //
    // If the KeyValue was invalid, return the flash entry point.
    //
    if (sciaGetWordData((uint16*)LOAD_ADDRESS) != 0x08AA)
    {
        return -1;
    }

    //
    // The next 32 bits specify the starting address. Read the starting address
    // and store it in EntryAddr
    //
    EntryAddr = getLongData((uint16*)(LOAD_ADDRESS+9));
    if((EntryAddr>address+0x100)||(EntryAddr<address-0x100))
    {
        UARTprintf("ldfuLoad:EntryAddr!=address,EntryAddr=%x\n\r",EntryAddr);
        return -1;
    }

    //
    // Return EntryAddr
    //
    return ldfuCopyData(address);
}


int16_t Erase_FlashSector(uint32_t u32StartAddress,uint32_t u32endAddress)
{
    uint32_t beginAddress=u32StartAddress;
    uint32_t endAddress=u32endAddress;
    uint32_t sectorAddress;
    uint16_t n = 0;
    uint16_t sectorSize;


    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusWordType oFlashStatusWord;

    EALLOW;

    for(n = GET_SECTION_NUMS(beginAddress); n <= GET_SECTION_NUMS(endAddress); n++ )
    {
       //
       // Get the current address
       //
       sectorAddress = secAddress[n];

       //
       // FindSize just returns 0x4000.
       //
       sectorSize = B_8KSector_u32length;

       //
       // Execute an erase operation using the Flash API
       //
       oReturnCheck = Fapi_issueAsyncCommandWithAddress(
               Fapi_EraseSector, (uint32_t *) sectorAddress);

       //
       // Wait until the Flash State Machine is ready
       //
       while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

       //
       // Perform a blank check on the recently erased sector to ensure it was erased
       //
       oReturnCheck = Fapi_doBlankCheck(
               (uint32_t *) sectorAddress, sectorSize,
               &oFlashStatusWord);

       //
       // Check the Flash API status for an error
       //
       if (oReturnCheck != Fapi_Status_Success)
       {
           UARTprintf("Erase_FlashSector:for(n = GET_SECTION_NUMS(beginAddress); n <= GET_SECTION_NUMS(endAddress); n++ )\n\r");
           EDIS;
           return -1;
       }

       //
       // Wait for the Flash State Machine
       //
       while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
       {
       }
   }

    EDIS;
    return 0;
}


uint32_t Write_Flash(uint32_t u32StartAddress,uint16_t *pu16DataBuffer,uint16_t  u16DataBufferSizeInWords)
{
    uint32_t u32Index = 0;
    uint16_t i = 0;
    uint32_t *Buffer32 = (uint32_t *)pu16DataBuffer;
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusWordType  oFlashStatusWord;

    ASSERT((u16DataBufferSizeInWords%8==0)||(u16DataBufferSizeInWords>=8));
    //ASSERT(((uint32_t)pu16DataBuffer&0x07) == 0);
    EALLOW;
    for(i=0, u32Index = u32StartAddress;
          u32Index < (u32StartAddress + u16DataBufferSizeInWords) ; i+= 8, u32Index+= 8)
       {
           oReturnCheck = Fapi_issueProgrammingCommand((uint32_t *)u32Index, pu16DataBuffer+i, 8,
                                                                                    0, 0, Fapi_AutoEccGeneration);

           // Wait until the Flash program operation is over
           while(Fapi_checkFsmForReady() == Fapi_Status_FsmBusy);

           if(oReturnCheck != Fapi_Status_Success)
           {
               UARTprintf("Write_Flash:Fapi_checkFsmForReady=%d\n\r",u32Index);
               EDIS;
               return 0;
           }

           // Verify the programmed values.  Check for any ECC errors.
           // The program command itself does a verify as it goes.
           // Hence program verify by CPU reads (Fapi_doVerify()) is optional.
           oReturnCheck = Fapi_doVerify((uint32_t *)u32Index,
                                        4, Buffer32+(i/2),
                                        &oFlashStatusWord);

           if(oReturnCheck != Fapi_Status_Success)
           {
               UARTprintf("Write_Flash:Fapi_doVerify=%d\n\r",u32Index);
               EDIS;
               return 0;
           }
       }
    EDIS;
    return u32Index;
}


//
// ldfuCopyData -  Erases and programs the appropriate flash bank. For the
//                 BANK0 build configuration, the function erases and programs
//                 bank 1. For the BANK1 build configuration, the function
//                 erases and programs bank 0. The steps taken are listed below:
//
//                 BANK0 Build Configurations
//                 1. Reads 'REV' value of Bank 1 and Bank 0
//                 2. Erases Sectors 2-15 of Bank 1
//                 3. Writes 'START' value to B1_START_ADD
//                 4. Programs Bank 1 and verifies the data
//                 5. Decrements Bank 1's revision value
//                 6. Writes 'KEY' value to B1_KEY_ADD and 'REV' value to
//                    B1_REV_ADD
//
//                 BANK1 Build Configurations
//                 1. Reads 'REV' value of Bank 1 and Bank 0
//                 2. Erases Sectors 2-15 of Bank 0
//                 3. Writes 'START' value to B0_START_ADD
//                 4. Programs Bank 0 and verifies the data
//                 5. Decrements Bank 0's revision value
//                 6. Writes 'KEY' value to B0_KEY_ADD and 'REV' value to
//                    B0_REV_ADD                 
//
tpHEADER BlockHeader;

int16_t ldfuCopyData(uint32_t startAdd)
{
    //
    // BlockSize is used to store the size of the block of data to be read in
    // DestAddr is used to store the address of where the block of data is
    // to be written to in flash
    //

    uint16_t BlockSize,tempBlockSize;
    uint32_t DestAddr;
    //
    // Buffer: Used to program data to flash
    //
    uint16_t Buffer[BUFFER_SIZE];

    //
    // miniBuffer: Useful for 4-word access to flash
    //
    uint16_t miniBuffer[4];

    //
    // i, j, k, and n are used to iterate through each block of data
    //
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    uint16_t n = 0;

    //
    // sectorAddress: Used to store the address of a sector
    //
    uint32_t sectorAddress;

    //
    // sectorSize: Used to store the size of a sector
    //
    uint16_t sectorSize;

    //
    // wordData: Stores a word of data
    //
    uint16_t wordData;

    //
    // oReturnCheck: Used to store the status of the Flash API after operations
    // are performed
    //
    Fapi_StatusType oReturnCheck;

    //
    // oFlashStatusWord: used during a blank check and other Flash API operations
    //
    Fapi_FlashStatusWordType oFlashStatusWord;

    //
    // beginAddress: Used to specify the index of the address in the
    //               secAddress array at which erasing needs to start
    //
    uint32_t beginAddress;

    //
    // endAddress: Used to specify the index of the address in the
    //             secAddress array at which erasing needs to end
    //
    uint32_t endAddress;

    uint16_t *ptdata;
    //
    // startAdd: Used to store the addres at which the 'START' value will be written
    //
    switch(startAdd)
    {
        case UB_ADDRES:
            beginAddress = UB_ADDRES;
            endAddress = UB_ADDRES_END;
            break;
        case UL_ADDRES:
            beginAddress = UL_ADDRES;
            endAddress = UL_ADDRES_END;
            break;
        case DSP_ADDRES:
            beginAddress = DSP_ADDRES;
            endAddress =  DSP_ADDRES_END;
            break;
        default:
            UARTprintf("ldfuCopyData:switch(startAdd)  startAdd=%x\n\r",startAdd);
            return -1;
    }
    DestAddr = startAdd;


    //
    // Ensure BUFFER_SIZE is a valid value
    //
     assert(BUFFER_SIZE % 4 == 0);

    //
    // Enable writing to the EALLOW protected registers
    //
    EALLOW;

    //
    // The for loop iterates through the secAddress array and erases the
    // sector that corresponds to each specified index. For the BANK1 configuration,
    // index 2-15. For the BANK0 configuration, index 18-31.
    //
    for(n = GET_SECTION_NUMS(beginAddress); n <= GET_SECTION_NUMS(endAddress); n++ )
    {
        //
        // Get the current address
        //
        sectorAddress = secAddress[n];

        //
        // FindSize just returns 0x4000. 
        //
        sectorSize = B_8KSector_u32length;

        //
        // Execute an erase operation using the Flash API
        //
        oReturnCheck = Fapi_issueAsyncCommandWithAddress(
                Fapi_EraseSector, (uint32_t *) sectorAddress);

        //
        // Wait until the Flash State Machine is ready
        //
        while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

        //
        // Perform a blank check on the recently erased sector to ensure it was erased
        //
        oReturnCheck = Fapi_doBlankCheck(
                (uint32_t *) sectorAddress, sectorSize,
                &oFlashStatusWord);

        //
        // Check the Flash API status for an error
        //
        if (oReturnCheck != Fapi_Status_Success)
        {
            UARTprintf("ldfuCopyData:Fapi_EraseSector secAddress[n] = %d\n\r",secAddress[n]);
            EDIS;
            return -1;
        }

        //
        // Wait for the Flash State Machine
        //
        while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy)
        {
        }
    }
    //
    // Disable writing to the EALLOW protected registers
    //
    EDIS;



    //
    // Enable writing to the EALLOW protected registers
    //
    EALLOW;

    //
    // While the block size is > 0 copy the data
    // to the DestAddr. It is assumed the DestAddr is a valid
    // memory location; however, a value less than the
    // reserved memory boundary for the bank will halt operation

    ptdata=getBlockHeader(&BlockHeader);

    BlockSize = BlockHeader.BlockSize;
    tempBlockSize = BlockSize;
    while (ptdata != NULL)
    {

        //
        // Check if the destination address is less than B0_RESERVED
        //
        if(BlockHeader.DestAddr < beginAddress)
        {
            //
            // Loop Forever. In the terminal of the Serial Flash Programmer,
            // new bytes will stop showing
            //
            UARTprintf("ldfuCopyData:BlockHeader.DestAddr < beginAddress,BlockHeader.DestAddr=%x\n\r",BlockHeader.DestAddr);
            EDIS;
            return -1;
        }

        //
        // Iterate through the block of data in order to program the data
        // in flash and verify it
        //


        n = BlockHeader.DestAddr%4;
        BlockHeader.DestAddr = BlockHeader.DestAddr - n;

        BlockHeader.BlockSize = BlockHeader.BlockSize + n;
        BlockSize = BlockHeader.BlockSize;
        BlockHeader.BlockSize = (BlockHeader.BlockSize/4)*4;
        DestAddr = BlockHeader.DestAddr+BlockHeader.BlockSize;


        for (i = 0; i < BlockHeader.BlockSize; i += 0)
        {
            //
            // If the size of the block of data is less than the size of the buffer,
            // then fill the buffer with the block of data and pad the remaining
            // elements
            //
            if (BlockHeader.BlockSize < BUFFER_SIZE)
            {
                //
                // Receive the block of data one word at a time and place it in
                // the buffer
                //
                i += n;
                for (j = n; j < BlockHeader.BlockSize; j++)
                {
                    //
                    // Receive one word of data
                    //
                    wordData = (*getWordData)(ptdata);
                    ptdata++;

                    //
                    // Put the word of data in the buffer
                    //
                    Buffer[j] = wordData;

                    //
                    // Increment i to keep track of how many words have been received
                    //
                    i++;
                }
                n=0;

                //
                // Pad the remaining elements of the buffer
                //
                for (j = BlockHeader.BlockSize; j < BUFFER_SIZE; j++)
                {
                    //
                    // Put 0xFFFF into the current element of the buffer. OxFFFF is equal to erased
                    // data and has no effect
                    //
                    Buffer[j] = 0xFFFF;
                }
            }

            //
            // BlockHeader.BlockSize >= BUFFER_SIZE
            //
            else
            {
                //
                // Less than one BUFFER_SIZE left
                //
                if ((BlockHeader.BlockSize - i) < BUFFER_SIZE)
                {
                    //
                    // Fill Buffer with rest of data
                    //
                    for (j = 0; j < BlockHeader.BlockSize - i; j++)
                    {
                        //
                        // Receive one word of data
                        //
                        wordData = (*getWordData)(ptdata);
                        ptdata++;

                        //
                        // Put the word of data into the current element of Buffer
                        //
                        Buffer[j] = wordData;
                    }

                    //
                    // Increment i outside here so it doesn't affect loop above
                    //
                    i += j;

                    //
                    // Fill the rest with 0xFFFF
                    //
                    for (; j < BUFFER_SIZE; j++)
                    {
                        Buffer[j] = 0xFFFF;
                    }
                }
                else
                {
                    //
                    // Fill up like normal, up to BUFFER_SIZE
                    //
                    i += n;
                    for (j = n; j < BUFFER_SIZE; j++)
                    {
                        wordData = (*getWordData)(ptdata);
                        ptdata++;
                        Buffer[j] = wordData;
                        i++;
                    }
                    n=0;
                }
            }

            //
            // Fill miniBuffer with the data in Buffer in order to program the data
            // to flash; miniBuffer takes data from Buffer, 4 words at a time.
            //
            for (k = 0; k < (BUFFER_SIZE / 4); k++)
            {
                //
                // Get 4 words from Buffer
                //
                miniBuffer[0] = Buffer[k * 4 + 0];
                miniBuffer[1] = Buffer[k * 4 + 1];
                miniBuffer[2] = Buffer[k * 4 + 2];
                miniBuffer[3] = Buffer[k * 4 + 3];

                //
                // Check that miniBuffer is not already all erased data
                //
                if (!((miniBuffer[0] == 0xFFFF) && (miniBuffer[1] == 0xFFFF)
                        && (miniBuffer[2] == 0xFFFF)
                        && (miniBuffer[3] == 0xFFFF)))
                {
                    //
                    // Program 4 words at once, 64-bits
                    //
                    oReturnCheck = Fapi_issueProgrammingCommand(
                            (uint32_t *) BlockHeader.DestAddr, miniBuffer,
                            sizeof(miniBuffer), 0, 0, Fapi_AutoEccGeneration);
                    //
                    // Wait for the programming operation to finish
                    //
                    while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy){};

                    //
                    // Check for an error
                    //
                    if (oReturnCheck != Fapi_Status_Success)
                    {
                        //
                        // First fail; store the error type and address of the error
                        //

                        UARTprintf("ldfuCopyData:k = 0; k < (BUFFER_SIZE / 4); k++\n\r");
                        EDIS;
                        return -1;
                    }

                    //
                    // oFlashStatus = Fapi_getFsmStatus();
                    // Verify the data that was programmed by using the Flash API
                    //
                    for (j = 0; j < 4; j += 2)
                    {
                        uint32_t toVerify = miniBuffer[j + 1];
                        toVerify = toVerify << 16;
                        toVerify |= miniBuffer[j];

                        //
                        // Perform a verify operation with the Flash API
                        //
                        oReturnCheck = Fapi_doVerify(
                                (uint32_t *) (BlockHeader.DestAddr + j), 1,
                                (uint32_t *) (&toVerify), &oFlashStatusWord);

                        //
                        // Check for an error with the verify operation
                        //
                        if (oReturnCheck != Fapi_Status_Success)
                        {
                            UARTprintf("ldfuCopyData: for (j = 0; j < 4; j += 2)\n\r");
                            EDIS;
                            return -1;
                        }
                    //
                    // for j; for Fapi_doVerify
                    //
                    }
                //
                // check if miniBuffer does not contain all already erased data
                //
                }
                BlockHeader.DestAddr += 0x4;

            //
            // for(int k); loads miniBuffer with Buffer elements
            //
            }
        }

        //
        // Get the size of the next block of data
        //


        if(BlockSize>BlockHeader.BlockSize)
        {
            for (j = 0; j < BlockSize - BlockHeader.BlockSize; j++)
            {
                //
                // Receive one word of data
                //
                wordData = (*getWordData)(ptdata);
                ptdata++;

                //
                // Put the word of data into the current element of Buffer
                //
                Buffer[j] = wordData;
            }
            for(;j<4;j++)
            {
                Buffer[j] = (uint16_t)0xFFFF;
            }
        }
        tempBlockSize = BlockHeader.BlockSize;
        ptdata=getBlockHeader(&BlockHeader);
        //
        // Get the destination address of the block of data
        //
        if(BlockSize>tempBlockSize)
        {
            if(BlockHeader.DestAddr>=DestAddr+4)
            {
                miniBuffer[0] = Buffer[0];
                miniBuffer[1] = Buffer[1];
                miniBuffer[2] = Buffer[2];
                miniBuffer[3] = Buffer[3];

                //
                // Check that miniBuffer is not already all erased data
                //
                if (!((miniBuffer[0] == 0xFFFF) && (miniBuffer[1] == 0xFFFF)
                        && (miniBuffer[2] == 0xFFFF)
                        && (miniBuffer[3] == 0xFFFF)))
                {
                    //
                    // Program 4 words at once, 64-bits
                    //
                    oReturnCheck = Fapi_issueProgrammingCommand(
                            (uint32_t *) DestAddr, miniBuffer,
                            sizeof(miniBuffer), 0, 0, Fapi_AutoEccGeneration);
                    //
                    // Wait for the programming operation to finish
                    //
                    while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy){};

                    //
                    // Check for an error
                    //
                    if (oReturnCheck != Fapi_Status_Success)
                    {
                        //
                        // First fail; store the error type and address of the error
                        //
                        UARTprintf("error:if(BlockSize>BlockHeader.BlockSize)\n\r");
                        EDIS;
                        return -1;
                    }

                    //
                    // oFlashStatus = Fapi_getFsmStatus();
                    // Verify the data that was programmed by using the Flash API
                    //
                    for (j = 0; j < 4; j += 2)
                    {
                        uint32_t toVerify = miniBuffer[j + 1];
                        toVerify = toVerify << 16;
                        toVerify |= miniBuffer[j];

                        //
                        // Perform a verify operation with the Flash API
                        //
                        oReturnCheck = Fapi_doVerify(
                                (uint32_t *) (DestAddr + j), 1,
                                (uint32_t *) (&toVerify), &oFlashStatusWord);

                        //
                        // Check for an error with the verify operation
                        //
                        if (oReturnCheck != Fapi_Status_Success)
                        {
                            UARTprintf("error:if(BlockSize>BlockHeader.BlockSize)\n\r");
                            EDIS;
                            return -1;
                        }
                    }
                }
            }
        }

    }

    //
    // Disable writing to the EALLOW protected registers
    //
    EDIS;

    return 1;
}

//
// getLongData- Fetches a 32-bit value from the peripheral
//              input stream.
//
uint32_t getLongData(uint16_t *p)
{
    uint32_t longData;

    //
    // Fetch the upper 1/2 of the 32-bit value
    //
    longData = ((uint32_t) (*getWordData)(p) << 16);

    //
    // Fetch the lower 1/2 of the 32-bit value
    //
    longData |= (uint32_t) (*getWordData)(p+1);

    return longData;
}



//
// End of file
//
