#include "manual_controls.h"

bool debouncedKeyState[eTOTAL_BUTTON_COUNT] = {false, false, false, false};
uint8_t count[eTOTAL_BUTTON_COUNT] = {0,0,0,0};

void InitializeManualControls(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_FRONT_LEFT_ACT_PORT, BOARD_FRONT_LEFT_ACT_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_FRONT_RIGHT_ACT_PORT, BOARD_FRONT_RIGHT_ACT_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_MANUAL_MODE_PORT, BOARD_MANUAL_MODE_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_AUTO_MODE_PORT, BOARD_AUTO_MODE_PIN, kPORT_InterruptFallingEdge);

    EnableIRQ(PORTC_IRQn);

    GPIO_PinInit(BOARD_FRONT_LEFT_ACT_GPIO, BOARD_FRONT_LEFT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_FRONT_RIGHT_ACT_GPIO, BOARD_FRONT_RIGHT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_MANUAL_MODE_GPIO, BOARD_MANUAL_MODE_PIN, &general_config);
    GPIO_PinInit(BOARD_AUTO_MODE_GPIO, BOARD_AUTO_MODE_PIN, &general_config);
}

#define REGISTER_MSEC 20

void ButtonValueDebounce(button_t button, uint32_t pollingRate){
	bool rawState = false;
	switch(button){
		case eFrontLeftActuate:
			rawState = GPIO_PinRead(BOARD_FRONT_LEFT_ACT_GPIO, BOARD_FRONT_LEFT_ACT_PIN) == 0;
			break;
		case eFrontRightActuate:
			rawState = GPIO_PinRead(BOARD_FRONT_RIGHT_ACT_GPIO, BOARD_FRONT_RIGHT_ACT_PIN) == 0;
			break;
		case eManualMode:
			rawState =  GPIO_PinRead(BOARD_MANUAL_MODE_GPIO, BOARD_MANUAL_MODE_PIN) == 0;
			break;
		case eAutoMode:
			rawState = GPIO_PinRead(BOARD_AUTO_MODE_GPIO, BOARD_AUTO_MODE_PIN) == 0;
			break;
		default:
			break;
	}
	if(rawState == debouncedKeyState[button]){
		count[button] = REGISTER_MSEC / pollingRate;
	}
	else{
		if(--count[button] == 0){
			debouncedKeyState[button] = rawState;
			count[button] = REGISTER_MSEC / pollingRate;
		}
	}
}

void UpdateManualControls(uint32_t pollingRate)
{
	button_t button;

	for(button = eFrontLeftActuate; button < eTOTAL_BUTTON_COUNT; button++){
		ButtonValueDebounce(button, pollingRate);
	}
}

void getManualControlValues(bool *frontLeftActuate, bool *frontRightActuate, bool *manualMode, bool *autoMode)
{
	*frontLeftActuate = debouncedKeyState[eFrontLeftActuate];
	*frontRightActuate = debouncedKeyState[eFrontRightActuate];
	*manualMode = debouncedKeyState[eManualMode];
	*autoMode = debouncedKeyState[eAutoMode];
}
