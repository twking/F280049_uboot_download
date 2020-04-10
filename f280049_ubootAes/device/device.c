//#############################################################################
//
// FILE:   device.c
//
// TITLE:  Device setup for examples.
//
//#############################################################################
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
//#############################################################################

//
// Included Files
//
#include "device.h"
#include "driverlib.h"

#ifdef __cplusplus
using std::memcpy;
#endif


#ifdef _FLASH
#include <stddef.h>

extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadEnd;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
extern uint16_t RamfuncsRunEnd;
extern uint16_t RamfuncsRunSize;
#endif

#define PASS 0
#define FAIL 1


//*****************************************************************************
//
// Function to initialize the device. Primarily initializes system control to a
// known state by disabling the watchdog, setting up the SYSCLKOUT frequency,
// and enabling the clocks to the peripherals.
//
//*****************************************************************************

void Device_init(void)
{
    //uint32_t clk;
    //
    // Disable the watchdog
    //
    SysCtl_disableWatchdog();

#ifdef _FLASH
    //
    // Copy time critical code and flash setup code to RAM. This includes the
    // following functions: InitFlash();
    //
    // The RamfuncsLoadStart, RamfuncsLoadSize, and RamfuncsRunStart symbols
    // are created by the linker. Refer to the device .cmd file.
    //
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif
    //
    // Set up PLL control and clock dividers
    //
    SysCtl_setClock(DEVICE_SETCLOCK_CFG);

    //
    // Make sure the LSPCLK divider is set to the default (divide by 4)
    //
    SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_4);

    //
    // These asserts will check that the #defines for the clock rates in
    // device.h match the actual rates that have been configured. If they do
    // not match, check that the calculations of DEVICE_SYSCLK_FREQ and
    // DEVICE_LSPCLK_FREQ are accurate. Some examples will not perform as
    // expected if these are not correct.
    //
    //clk = SysCtl_getClock(DEVICE_OSCSRC_FREQ);
    //clk = SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ);

    //ASSERT(SysCtl_getClock(DEVICE_OSCSRC_FREQ) == DEVICE_SYSCLK_FREQ);
    //ASSERT(SysCtl_getLowSpeedClock(DEVICE_OSCSRC_FREQ) == DEVICE_LSPCLK_FREQ);


    //Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, DEVICE_FLASH_WAITSTATES);
    //
    // Turn on all peripherals
    //
    Device_enableAllPeripherals();
    //SCI_setConfig(SCIA_BASE,DEVICE_LSPCLK_FREQ,115200,SCI_CONFIG_WLEN_8|SCI_CONFIG_STOP_ONE|SCI_CONFIG_PAR_NONE);
}

//*****************************************************************************
//
// Function to turn on all peripherals, enabling reads and writes to the
// peripherals' registers.
//
// Note that to reduce power, unused peripherals should be disabled.
//
//*****************************************************************************
void Device_enableAllPeripherals(void)
{
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CLA1);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DMA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER0);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER1);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TIMER2);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_HRPWM);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM1);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM2);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM3);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM4);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM5);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM6);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM7);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EPWM8);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP1);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP2);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP3);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP4);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP5);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP6);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ECAP7);

   // SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP1);
   // SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_EQEP2);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SD1);

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIB);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SPIB);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_I2CA);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CANB);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCB);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCC);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS1);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS2);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS3);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS4);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS5);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS6);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_CMPSS7);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA1);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA2);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA3);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA4);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA5);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA6);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PGA7);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DACA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_DACB);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_LINA);

    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_PMBUSA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_FSITXA);
    //SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_FSIRXA);
}

//*****************************************************************************
//
// Function to disable pin locks and enable pullups on GPIOs.
//
//*****************************************************************************
void Device_initGPIO(void)
{
    //
    // Disable pin locks.
    //
    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF);
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF);
}

//
// Device_intSCIA - Initialize the SCI-A port for communications with the host.
//
void Device_intSCIA(void)
{
    //
    // Enable the SCI-A clocks
    //
    EALLOW;

    //
    // Device_enableAllPeripherals enabled PCLKCR7.SCI_A already.
    //

    //
    // TRM, 0x0007 -> scaler of 14. OSCLOK low speed scaling for SCI.
    //

    //SysCtl_setLowSpeedClock(SYSCTL_LSPCLK_PRESCALE_14);

    // reset SCI channels.
    SCI_resetChannels(SCIA_BASE);

    //
    // 1 stop bit, No parity, 8-bit character
    // No loopback
    //
    // CLK speed and Baud rate get overwritten in autobaud_lock function later.
    //
    SCI_setConfig(
            SCIA_BASE, DEVICE_LSPCLK_FREQ, DEFAULT_BAUD,
            SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE);


    //
    // Enable TX, RX, Use internal SCICLK
    //
    SCI_enableModule(SCIA_BASE);

    //
    // Disable RxErr, Sleep, TX Wake,
    // Disable Rx Interrupt, Tx Interrupt
    //
    SCI_disableInterrupt(
            SCIA_BASE,
            SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY | SCI_INT_RXERR | SCI_INT_TXRDY);

    //
    // Relinquish SCI-A from reset
    //
    SCI_performSoftwareReset(SCIA_BASE);
    EDIS;

    // pick SCIA TX and RX GPIO pin according to BootMode.

    //
    // Configure GPIO29 as SCITXDA (Output pin)
    // Configure GPIO28 as SCIRXDA (Input pin)
    //

    //
    // GPIO29 is the SCI Tx pin.
    //
    GPIO_setMasterCore(29, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_29_SCITXDA);
    GPIO_setDirectionMode(29, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(29, GPIO_QUAL_ASYNC);

    //
    // GPIO28 is the SCI Rx pin.
    //
    GPIO_setMasterCore(28, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_28_SCIRXDA);
    GPIO_setDirectionMode(28, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(28, GPIO_QUAL_ASYNC);

}

//*****************************************************************************
//
// Error handling function to be called when an ASSERT is violated
//
//*****************************************************************************
void __error__(char *filename, uint32_t line)
{
    //
    // An ASSERT condition was evaluated as false. You can use the filename and
    // line parameters to determine what went wrong.
    //
    ESTOP0;
}
