#ifndef __GPS_H__
#define __GPS_H__

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_ftm.h"
#include "IrqHandler.h"

#include "Adafruit_PMTK.h"

#ifdef __cplusplus
extern "C" {
#endif

bool InitializeGPS(void);
double getGpsVelocity(void);

#ifdef __cplusplus
}
#endif

#endif //__GPS_H__
