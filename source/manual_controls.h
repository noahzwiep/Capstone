#ifndef __MANUAL_CONTROLS_H__
#define __MANUAL_CONTROLS_H__

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

typedef enum _buttons
{
	eFrontLeftActuate = 0U,
	eFrontRightActuate = 1U,
	eManualMode = 2U,
	eAutoMode = 3U,
	eTOTAL_BUTTON_COUNT
} button_t;

void InitializeManualControls(void);
void UpdateManualControls(uint32_t pollingRate);
void getManualControlValues(bool *frontLeftActuate, bool *frontRightActuate, bool *manualMode, bool *autoMode);

#ifdef __cplusplus
}
#endif

#endif //__MANUAL_CONTROLS_H__
