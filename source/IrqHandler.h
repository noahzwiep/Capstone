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

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#define GPS_BUFFER_SIZE 8

void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);
void UART3_SERIAL_RX_TX_IRQHANDLER(void);

extern bool bUpdateAccelerometer;
extern bool b1kHzFlag;
extern bool b5kHzFlag;
extern bool b10kHzFlag;

extern uint32_t capturedCounterFrontLeftHall;
extern uint32_t capturedCounterFrontRightHall;
extern uint32_t capturedCounterRearHall;

extern uint8_t rxBuffer[GPS_BUFFER_SIZE];

extern uint64_t g_systickCounter;

extern bool g_bFrontLeftActuate;
extern bool g_bFrontRightActuate;
extern bool g_bManualMode;
extern bool g_bAutoMode;


#endif //__IRQ_HANDLER_H__
