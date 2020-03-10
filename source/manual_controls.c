#include "manual_controls.h"

void InitializeManualControls(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_FRONT_LEFT_ACT_PORT, BOARD_FRONT_LEFT_ACT_PIN, kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(BOARD_FRONT_RIGHT_ACT_PORT, BOARD_FRONT_RIGHT_ACT_PIN, kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(BOARD_MANUAL_MODE_PORT, BOARD_MANUAL_MODE_PIN, kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(BOARD_AUTO_MODE_PORT, BOARD_AUTO_MODE_PIN, kPORT_InterruptRisingEdge);

    EnableIRQ(PORTC_IRQn);

    GPIO_PinInit(BOARD_FRONT_LEFT_ACT_GPIO, BOARD_FRONT_LEFT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_FRONT_RIGHT_ACT_GPIO, BOARD_FRONT_RIGHT_ACT_PIN, &general_config);
    GPIO_PinInit(BOARD_MANUAL_MODE_GPIO, BOARD_MANUAL_MODE_PIN, &general_config);
    GPIO_PinInit(BOARD_AUTO_MODE_GPIO, BOARD_AUTO_MODE_PIN, &general_config);
}
