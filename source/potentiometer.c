#include "potentiometer.h"

adc16_config_t adc16ConfigStruct;
adc16_channel_config_t adc16ChannelConfigStruct;

void InitializePot(void)
{
    ADC16_GetDefaultConfig(&adc16ConfigStruct);
    ADC16_Init(ADC0, &adc16ConfigStruct);

    ADC16_EnableHardwareTrigger(POT_BASE, false); /* Make sure the software trigger is used. */
    ADC16_DoAutoCalibration(POT_BASE);

    adc16ChannelConfigStruct.channelNumber = POT_USER_CHANNEL;
    adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = false;
    adc16ChannelConfigStruct.enableDifferentialConversion = false;
}

/*Returns the mV value of the potentiometer*/
uint32_t getPotValue(void)
{
    ADC16_SetChannelConfig(POT_BASE, POT_CHANNEL_GROUP, &adc16ChannelConfigStruct);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(POT_BASE, POT_CHANNEL_GROUP)))
    {
    }
    /*System Voltage is 3290 mV, divide by 2^12 as default ADC is 12 bits*/
    return (uint32_t)(3290*ADC16_GetChannelConversionValue(POT_BASE, POT_CHANNEL_GROUP) >> 12);
}

uint32_t getPotAngle(void)
{
	return getPotValue()*312/3290;
}
