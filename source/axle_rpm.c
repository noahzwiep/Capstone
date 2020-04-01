#include "axle_rpm.h"

uint32_t prevCounter[eAXLE_COUNT] = {0,0,0};
uint32_t axleRpm[eAXLE_COUNT] = {0,0,0};

void InitializeAxleRpm(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_FRONT_RIGHT_RPM_PORT, BOARD_FRONT_RIGHT_RPM_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_FRONT_LEFT_RPM_PORT, BOARD_FRONT_LEFT_RPM_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_REAR_RPM_PORT, BOARD_REAR_RPM_PIN, kPORT_InterruptFallingEdge);

    EnableIRQ(PORTD_IRQn);

    GPIO_PinInit(BOARD_FRONT_RIGHT_RPM_GPIO, BOARD_FRONT_LEFT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_FRONT_LEFT_RPM_GPIO, BOARD_FRONT_RIGHT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_REAR_RPM_GPIO, BOARD_MANUAL_MODE_PIN, &general_config);
}

void UpdateAxleRpm(uint32_t updateHz)
{
	int64_t countDifference;
	axle_t axle;
	for(axle = eFrontLeftAxle; axle < eAXLE_COUNT; axle++){
		switch(axle){
			case eFrontLeftAxle:
				countDifference = capturedCounterFrontLeftHall - prevCounter[axle];
				/*Deal with overflow*/
				if(countDifference < 0){
					countDifference = capturedCounterFrontLeftHall + (4294967295-prevCounter[axle]);
				}
				prevCounter[axle] = capturedCounterFrontLeftHall;
				break;
			case eFrontRightAxle:
				countDifference = capturedCounterFrontRightHall - prevCounter[axle];
				/*Deal with overflow*/
				if(countDifference < 0){
					countDifference = capturedCounterFrontRightHall + (4294967295-prevCounter[axle]);
				}
				prevCounter[axle] = capturedCounterFrontRightHall;
				break;
			case eRearAxle:
				countDifference = capturedCounterRearHall - prevCounter[axle];
				/*Deal with overflow*/
				if(countDifference < 0){
					countDifference = capturedCounterRearHall + (4294967295-prevCounter[axle]);
				}
				prevCounter[axle] = capturedCounterRearHall;
				break;
			default:
				break;
		}

		axleRpm[axle] = countDifference*updateHz;
	}
}
