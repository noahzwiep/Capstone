#ifndef _MOTOR_H_
#define _MOTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_ftm.h"

typedef struct _FTM_Instance
{
	FTM_Type * FTM_BASEADDR;
	ftm_chnl_t FTM_CHANNEL;
	ftm_interrupt_enable_t FTM_INT;
	IRQn_Type FTM_IRQ_ID;
	ftm_status_flags_t FTM_CHANNEL_FLAG;
} FTM_Instance_t;

typedef enum _motor_direction
{
	eDirectionNotSet = 0U,
	eDirectionForward = 1U,
	eDirectionReverse = 2U,
	eTOTAL_DIRECTION_COUNT
} motor_direction_t;

typedef enum _motors
{
	eLeftMotor = 0,
	eRightMotor = 1,
	eTOTAL_MOTOR_COUNT
} motor_t;

typedef enum _limit_switches
{
	eLimitSwitchFrontLeft = 0U,
	eLimitSwitchFrontRight = 1U,
	eLIMIT_SWITCH_COUNT
} limit_t;

#define MAX_MOTOR_DEGREE 150

/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

/* Define IRQ Handlers Here*/
#define BOARD_FTM_LEFT_IRQ_HANDLER 	FTM3_IRQHandler
#define BOARD_FTM_RIGHT_IRQ_HANDLER FTM0_IRQHandler

/*Global Variables*/
extern motor_direction_t g_leftMotorDirection;
extern volatile uint16_t g_leftMotorPWM;
extern motor_direction_t g_rightMotorDirection;
extern volatile uint16_t g_rightMotorPWM;
extern volatile int64_t pulseCount[eTOTAL_MOTOR_COUNT];

/*Function Definitions*/
void InitializeMotors(void);
void HomeMotor(motor_t motor, bool wait);
void HomeMotors(bool wait);
void UpdatePwm(motor_t motor, int32_t new_pwm);
void UpdateMotorDirection(motor_t motor, motor_direction_t direction);
void stopMotor(motor_t motor);
void turnDegrees(motor_t motor, int32_t degrees, uint16_t pwm);
void turnDegreesPID(motor_t motor, int32_t degrees);
void goToDegree(motor_t motor, int32_t degree);
void goToDegreePID(motor_t motor, int32_t degree);

void UpdateLimitSwitches(uint32_t pollingRate);
bool isMotorHome(motor_t motor);

#ifdef __cplusplus
}
#endif

#endif
