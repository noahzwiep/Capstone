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
#include "manual_controls.h"
#include "axle_rpm.h"
#include "gps.h"
#include "IrqHandler.h"

#ifdef __cplusplus
extern "C"
{
#endif

// C header here
//Simulink includes
#include "rtmodel.h"

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

    /*Setup 1kHz Interrupt*/
    SysTick_Config(SystemCoreClock / 1000U);
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

    /*Initialize Timer Interrupts*/
    InitializeTimers();

    /*Initialize Hardware*/
    InitializePot();
    InitializeManualControls();
    //InitializeAxleRpm();
    bool bInitAccel = InitializeAccelerometer();
    if(!bInitAccel){
    	while(1){}
    }
    InitializeGPS();
    InitializeMotors();

    //Initialize Simulink code
    AWDControl_initialize();

    /*Main Processing Loop*/
    uint32_t potAngle = 0;
    double frontLeftMotorActuatePercent = 0;
    double frontRightMotorActuatePercent = 0;
    bool leftManualOverride = false;
    bool rightManualOverride = false;
    bool manualMode = false;
    bool autoMode = false;
    bool bError = false;
    float ax, ay, az;
    float yaw,pitch,roll;

    while(1) {
    	if(b1kHzFlag){
    		bError = false;

    		/*Update Button Debouncing with rate of 1ms*/
    		UpdateLimitSwitches(1);
    		UpdateManualControls(1);
    		//UpdateAxleRpm(1);

    		if(isMotorHome(eLeftMotor)){
    			pulseCount[eLeftMotor] = 0;
    		} else{
    			if(pulseCount[eLeftMotor] == 0){
    				bError = true;
    			}
    		}
    		if(isMotorHome(eRightMotor)){
    			pulseCount[eRightMotor] = 0;
    		} else{
    			if(pulseCount[eRightMotor] == 0){
    				bError = true;
    			}
    		}

    		rtU.m_steeringAngle = potAngle;
    		rtU.m_rearAxleRPM = axleRpm[eRearAxle];
    		rtU.m_frontLeftRPM = axleRpm[eFrontLeftAxle];
    		rtU.m_frontRightRPM = axleRpm[eFrontRightAxle];
    		rtU.m_yawRate = yaw;
    		rtU.m_accelerationLongitudinal = ax;
    		rtU.m_accelerationLateral = ay;
    		rtU.m_GPSvehicleSpeed = getGpsVelocity();
    		rtU.m_activateLeft = leftManualOverride;
    		rtU.m_activateRight = rightManualOverride;
    		rtU.m_controlMode = (uint8_t)(manualMode | (uint8_t)(autoMode<<1));
    		rtU.m_frontLeftSwitch = isMotorHome(eLeftMotor);
    		rtU.m_frontRightSwitch = isMotorHome(eRightMotor);


    		//Call 1ms tasks for Simulink code
    		AWDControl_step();

    		//NOTE: Update all local variables by copying the output values from rtY after calling the step function
    		frontLeftMotorActuatePercent = rtY.c_frontLeftMotorActuate;
    		frontRightMotorActuatePercent = rtY.c_frontRightMotorActuate;

			bError = bError & (rtY.c_frontLeftMalfunction | rtY.c_frontRightMalfunction);

        	GPIO_PinWrite(BOARD_LED_ERROR_GPIO, BOARD_LED_ERROR_PIN, bError ? 1 : 0);

    		b1kHzFlag = false;
    	}
    	if(b5kHzFlag){
    		getManualControlValues(&leftManualOverride, &rightManualOverride, &manualMode, &autoMode);
    		GetMotionValues(&ax, &ay, &az, &yaw, &pitch, &roll);

    		goToDegreePID(eLeftMotor, frontLeftMotorActuatePercent*MAX_MOTOR_DEGREE);
    		goToDegreePID(eRightMotor, frontRightMotorActuatePercent*MAX_MOTOR_DEGREE);

    		b5kHzFlag = false;
    	}

    	if(b10kHzFlag){
        	potAngle = (90*potAngle + 10*getPotAngle())/100;

    		b10kHzFlag = false;
    	}
    }
    return 0 ;
}
