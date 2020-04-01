#ifndef __AXLE_RPM_H__
#define __AXLE_RPM_H__

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "IrqHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _axles
{
	eFrontLeftAxle,
	eFrontRightAxle,
	eRearAxle,
	eAXLE_COUNT
} axle_t;

extern uint32_t axleRpm[eAXLE_COUNT];

void InitializeAxleRpm(void);
/*Note: Ensure updateHz = the frequency you are capturing the counter value for the hall effects in IrqHandler.c
 * and call this function at that frequency!!*/
void UpdateAxleRpm(uint32_t updateHz);

#ifdef __cplusplus
}
#endif

#endif //__AXLE_RPM_H__
