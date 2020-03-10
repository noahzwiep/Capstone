#ifndef __IRQ_HANDLER_H__
#define __IRQ_HANDLER_H__

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "motor.h"

void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);

extern bool bUpdateAccelerometer;

#endif //__IRQ_HANDLER_H__
