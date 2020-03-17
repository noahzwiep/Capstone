/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MK64FN1M0xxx12_Project.cpp
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "motor.h"
#include "potentiometer.h"
#include "accelerometer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The Flextimer instance/channel used for board */
#define FTM_5kHz_BASEADDR FTM1
#define FTM_10kHz_BASEADDR FTM2

/* Interrupt number and interrupt handler for the FTM instance used */
#define FTM_5kHz_IRQ_NUM FTM1_IRQn
#define FTM_5kHz_HANDLER FTM1_IRQHandler

#define FTM_10kHz_IRQ_NUM FTM2_IRQn
#define FTM_10kHz_HANDLER FTM2_IRQHandler

/* Get source clock for FTM driver */
#define INTERRUPT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


bool b5kHzFlag = false;
bool b10kHzFlag = false;

#ifdef __cplusplus
extern "C" {
#endif
void FTM_5kHz_HANDLER(void)
{
    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(FTM_5kHz_BASEADDR, kFTM_TimeOverflowFlag);
    b5kHzFlag = true;
    __DSB();
}

void FTM_10kHz_HANDLER(void)
{
    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(FTM_10kHz_BASEADDR, kFTM_TimeOverflowFlag);
    b10kHzFlag = true;
    __DSB();
}
#ifdef __cplusplus
}
#endif

void InitializeTimers(void)
{
    ftm_config_t ftmInfo;

    /*Setup 5kHz Interrupt*/
    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(FTM_5kHz_BASEADDR, &ftmInfo);
    /* Setup Timer Period*/
    FTM_SetTimerPeriod(FTM_5kHz_BASEADDR, USEC_TO_COUNT(200U, INTERRUPT_SOURCE_CLOCK));
    /* Enable channel interrupt flag.*/
    FTM_EnableInterrupts(FTM_5kHz_BASEADDR, kFTM_TimeOverflowInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(FTM_5kHz_IRQ_NUM);

    FTM_StartTimer(FTM_5kHz_BASEADDR, kFTM_SystemClock);

    /*Setup 10kHz Interrupt*/
    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(FTM_10kHz_BASEADDR, &ftmInfo);
    /* Setup Timer Period*/
    FTM_SetTimerPeriod(FTM_10kHz_BASEADDR, USEC_TO_COUNT(100U, INTERRUPT_SOURCE_CLOCK));
    /* Enable channel interrupt flag.*/
    FTM_EnableInterrupts(FTM_10kHz_BASEADDR, kFTM_TimeOverflowInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(FTM_10kHz_IRQ_NUM);

    FTM_StartTimer(FTM_10kHz_BASEADDR, kFTM_SystemClock);
}


void delay(void)
{
    volatile uint32_t i = 0U;
    for (i = 0U; i < 8000000U; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitACCELPins();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    MotorsInit();
    InitializePot();
    InitializeTimers();

    bool accelInit = InitializeAccelerometer();
    if(accelInit){
    	while(1){}
    }

    uint32_t potValue = 0;
    while(1) {
    	if(b5kHzFlag){
    		//goToDegreePID(eLeftMotor, potValue*312/3290);
    		//goToDegreePID(eRightMotor, potValue*312/3290);
    		b5kHzFlag = false;
    	}

    	if(b10kHzFlag){
        	potValue = (90*potValue + 10*getPotValue())/100;
    		b10kHzFlag = false;
    	}
    }
    return 0 ;
}
