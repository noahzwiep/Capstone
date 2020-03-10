#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "MPU6050_6Axis_MotionApps20.h"
#include "IrqHandler.h"

bool InitializeAccelerometer(void);
void SetOffsets(void);
void UpdateMotion6Values(void);
void GetMotionValues(float * ax, float * ay, float * az, float * yaw, float * pitch, float * roll);



#endif //__ACCELEROMETER_H__
