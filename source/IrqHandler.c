#include "IrqHandler.h"

bool bUpdateAccelerometer = false;

#ifdef __cplusplus
extern "C" {
#endif

void PORTB_IRQHandler(void)
{
	uint32_t flags = GPIO_PortGetInterruptFlags(GPIOB);
	if(flags & (1<< BOARD_ENCODER1_PIN)){
		//If the encoder signal 1 goes low, and encoder signal 2 is still high, this implies going in one direction
		(GPIO_PinRead(BOARD_ENCODER2_GPIO, BOARD_ENCODER2_PIN) == 1) ? pulseCount[eLeftMotor]-- : pulseCount[eLeftMotor]++;
		GPIO_PortClearInterruptFlags(GPIOB, 1U << BOARD_ENCODER1_PIN);
	}
	if(flags & (1 << BOARD_MPU6050_INTERRUPT_PIN)){
		bUpdateAccelerometer = true;
		GPIO_PortClearInterruptFlags(GPIOB, 1U << BOARD_MPU6050_INTERRUPT_PIN);
	}
}

void PORTC_IRQHandler(void)
{
	uint32_t flags = GPIO_PortGetInterruptFlags(GPIOC);

	if(flags & (1<< BOARD_RIGHT_ENCODER_1_PIN)){
		//If the encoder signal 1 goes low, and encoder signal 2 is still high, this implies going in one direction
		(GPIO_PinRead(BOARD_RIGHT_ENCODER_2_GPIO, BOARD_RIGHT_ENCODER_2_PIN) == 1) ? pulseCount[eRightMotor]-- : pulseCount[eRightMotor]++;
		GPIO_PortClearInterruptFlags(GPIOC, 1U << BOARD_RIGHT_ENCODER_1_PIN);
	}
	if(flags & (1 << BOARD_FRONT_LEFT_ACT_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_LEFT_ACT_PIN));
	}
	if(flags & (1 << BOARD_FRONT_RIGHT_ACT_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_RIGHT_ACT_PIN));
	}
	if(flags & (1 << BOARD_MANUAL_MODE_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_MANUAL_MODE_PIN));
	}
	if(flags & (1 << BOARD_AUTO_MODE_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_AUTO_MODE_PIN));
	}
	if(flags & (1 << BOARD_FRONT_LEFT_LIMIT_PIN)){
		pulseCount[eLeftMotor] = 0;
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_LEFT_LIMIT_PIN));
	}
	if(flags & (1 << BOARD_FRONT_RIGHT_LIMIT_PIN)){
		pulseCount[eRightMotor] = 0;
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_RIGHT_LIMIT_PIN));
	}
}

#ifdef __cplusplus
}
#endif
