#include "IrqHandler.h"

bool bUpdateAccelerometer = false;
bool b1kHzFlag = false;
bool b5kHzFlag = false;
bool b10kHzFlag = false;

uint32_t counterFrontLeftHall = 0;
uint32_t counterFrontRightHall = 0;
uint32_t counterRearHall = 0;

uint32_t capturedCounterFrontLeftHall = 0;
uint32_t capturedCounterFrontRightHall = 0;
uint32_t capturedCounterRearHall = 0;

volatile uint16_t rxIndex = 0;
char gpsRxBuffer[GPS_BUFFER_SIZE];
uint8_t buffer[GPS_BUFFER_SIZE];

uint64_t g_systickCounter = 0;

bool g_bFrontLeftActuate = false;
bool g_bFrontRightActuate = false;
bool g_bManualMode = false;
bool g_bAutoMode = false;

bool bCheckGps = false;

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
    __DSB();
}

void PORTC_IRQHandler(void)
{
	uint32_t flags = GPIO_PortGetInterruptFlags(GPIOC);

	if(flags & (1<< BOARD_RIGHT_ENCODER_1_PIN)){
		//If the encoder signal 1 goes low, and encoder signal 2 is still high, this implies going in one direction
		(GPIO_PinRead(BOARD_RIGHT_ENCODER_2_GPIO, BOARD_RIGHT_ENCODER_2_PIN) == 1) ? pulseCount[eRightMotor]++ : pulseCount[eRightMotor]--;
		GPIO_PortClearInterruptFlags(GPIOC, 1U << BOARD_RIGHT_ENCODER_1_PIN);;
	}
	if(flags & (1 << BOARD_FRONT_LEFT_ACT_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_LEFT_ACT_PIN));
		g_bFrontLeftActuate = true;
	}
	if(flags & (1 << BOARD_FRONT_RIGHT_ACT_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_RIGHT_ACT_PIN));
		g_bFrontRightActuate = true;
	}
	if(flags & (1 << BOARD_MANUAL_MODE_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_MANUAL_MODE_PIN));
		g_bAutoMode = false;
		g_bManualMode = true;
	}
	if(flags & (1 << BOARD_AUTO_MODE_PIN)){
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_AUTO_MODE_PIN));
		g_bManualMode = false;
		g_bAutoMode = true;
	}
	if(flags & (1 << BOARD_FRONT_LEFT_LIMIT_PIN)){
		stopMotor(eLeftMotor);
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_LEFT_LIMIT_PIN));
	}
	if(flags & (1 << BOARD_FRONT_RIGHT_LIMIT_PIN)){
		stopMotor(eRightMotor);
		GPIO_PortClearInterruptFlags(GPIOC,(1 << BOARD_FRONT_RIGHT_LIMIT_PIN));
	}
    __DSB();
}

void PORTD_IRQHandler(void)
{
	uint32_t flags = GPIO_PortGetInterruptFlags(GPIOD);

	if(flags & (1<< BOARD_FRONT_RIGHT_RPM_PIN)){
		counterFrontRightHall++;
		GPIO_PortClearInterruptFlags(GPIOD,(1 << BOARD_FRONT_RIGHT_RPM_PIN));
	}

	if(flags & (1<< BOARD_FRONT_LEFT_RPM_PIN)){
		counterFrontLeftHall++;
		GPIO_PortClearInterruptFlags(GPIOD,(1 << BOARD_FRONT_LEFT_RPM_PIN));
	}

	if(flags & (1<< BOARD_REAR_RPM_PIN)){
		counterRearHall++;
		GPIO_PortClearInterruptFlags(GPIOD,(1 << BOARD_REAR_RPM_PIN));
	}
	__DSB();
}

void UART3_SERIAL_RX_TX_IRQHANDLER(void)
{
	uint8_t data;

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART3_PERIPHERAL))
	{
		data = UART_ReadByte(UART3_PERIPHERAL);

		if(data == '$')
		{
			if(buffer[3] == 'R'){
				for(int i = 0; i < GPS_BUFFER_SIZE; i++){
					gpsRxBuffer[i] = (i < rxIndex) ? buffer[i] : 32;
				}
				bCheckGps = true;
			}
			rxIndex = 0;
		}
		else
		{
			rxIndex++;
		}
		buffer[rxIndex] = data;
	}
}


void FTM_5kHz_HANDLER(void)
{
    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(FTM_5kHz_BASEADDR, kFTM_TimeOverflowFlag);
    b5kHzFlag = true;
    __DSB();
}

void FTM_10kHz_HANDLER(void)
{
    /* Clear interrupt flag.*/
    FTM_ClearStatusFlags(FTM_10kHz_BASEADDR, kFTM_TimeOverflowFlag);
    b10kHzFlag = true;

    /*Capture Hall Effect Counters*/
    capturedCounterFrontLeftHall = counterFrontLeftHall;
    capturedCounterFrontRightHall = counterFrontRightHall;
    capturedCounterRearHall = counterRearHall;

    __DSB();
}

void SysTick_Handler(void)
{
	g_systickCounter++;
	b1kHzFlag = true;
}


#ifdef __cplusplus
}
#endif
