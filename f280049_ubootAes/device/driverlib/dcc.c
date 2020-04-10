//###########################################################################
//
// FILE:   dcc.c
//
// TITLE:  C28x DCC driver.
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

#include "dcc.h"

//*****************************************************************************
//
// DCC_getRevisionNumber
//
//*****************************************************************************
uint16_t
DCC_getRevisionNumber(uint32_t base, DCC_RevisionNumber identifier)
{
    uint16_t number;

    //
    // Check the arguments.
    //
    ASSERT(DCC_isBaseValid(base));

    //
    //  Get specified revision number or scheme
    //
    if(identifier == DCC_REVISION_MINOR)
    {
        number = (HWREGH(base + DCC_O_REV) & DCC_REV_MINOR_M);
    }
    else if(identifier == DCC_REVISION_CUSTOM)
    {
        number = (HWREGH(base + DCC_O_REV) & DCC_REV_CUSTOM_M) >>
                 DCC_REV_CUSTOM_S;
    }
    else if(identifier == DCC_REVISION_MAJOR)
    {
        number = (HWREGH(base + DCC_O_REV) & DCC_REV_MAJOR_M) >>
                 DCC_REV_MAJOR_S;
    }
    else if(identifier == DCC_REVISION_DESIGN)
    {
        number = (HWREGH(base + DCC_O_REV) & DCC_REV_RTL_M) >>
                 DCC_REV_RTL_S;
    }
    else if(identifier == DCC_REVISION_FUNCTIONAL)
    {
        number = (HWREGH(base + DCC_O_REV + 2U) & (DCC_REG_WORD_MASK >> 4U));
    }
    else
    {
        //
        // Module Scheme
        //
        number = (HWREGH(base + DCC_O_REV + 2U) &
                  (uint16_t)(DCC_REV_SCHEME_M >> 16U)) >> 14U;
    }

    return(number);
}

