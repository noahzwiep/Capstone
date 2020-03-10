#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_adc16.h"

#define POT_BASE ADC0
#define POT_CHANNEL_GROUP 0U
#define POT_USER_CHANNEL 12U

void InitializePot(void);
uint32_t getPotValue(void);
uint32_t getPotAngle(void);

#ifdef __cplusplus
}
#endif

#endif //_POTENTIOMETER_H_
