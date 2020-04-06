#include "motor.h"

motor_direction_t g_motorDirection[eTOTAL_MOTOR_COUNT] = {eDirectionNotSet, eDirectionNotSet};
volatile uint16_t g_motorPwm[eTOTAL_MOTOR_COUNT] = {0, 0};
volatile int64_t pulseCount[eTOTAL_MOTOR_COUNT] = {0,0};
uint8_t stateCount[eLIMIT_SWITCH_COUNT] = {0,0};

/*The motors initialize to home position, which is why this is true*/
bool debouncedLimitState[eLIMIT_SWITCH_COUNT] = {true, true};

FTM_Instance_t LEFT_FTM;
FTM_Instance_t RIGHT_FTM;
FTM_Instance_t ftmStructs[eTOTAL_MOTOR_COUNT];

int64_t previousError[eTOTAL_MOTOR_COUNT] = {0,0};

void BOARD_FTM_LEFT_IRQ_HANDLER(void)
{
	uint32_t status = FTM_GetStatusFlags(LEFT_FTM.FTM_BASEADDR);
    if (status & LEFT_FTM.FTM_CHANNEL_FLAG)
    {
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(LEFT_FTM.FTM_BASEADDR, LEFT_FTM.FTM_CHANNEL_FLAG);
    }
    __DSB();
}

void BOARD_FTM_RIGHT_IRQ_HANDLER(void)
{
	uint32_t status = FTM_GetStatusFlags(RIGHT_FTM.FTM_BASEADDR);
    if (status & RIGHT_FTM.FTM_CHANNEL_FLAG)
    {
        /* Clear interrupt flag.*/
        FTM_ClearStatusFlags(RIGHT_FTM.FTM_BASEADDR, RIGHT_FTM.FTM_CHANNEL_FLAG);
    }
    __DSB();
}

static void Pwm_Init(FTM_Instance_t ftm_instance)
{
    ftm_config_t ftmInfo;
    ftm_chnl_pwm_signal_param_t ftmParam;

    /* Configure ftm params with frequency 24kHZ */
    ftmParam.chnlNumber = ftm_instance.FTM_CHANNEL;
    ftmParam.level = kFTM_HighTrue;
    ftmParam.dutyCyclePercent = 0U;
    ftmParam.firstEdgeDelayPercent = 0U;

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(ftm_instance.FTM_BASEADDR, &ftmInfo);

    FTM_SetupPwm(ftm_instance.FTM_BASEADDR, &ftmParam, 1U, kFTM_CenterAlignedPwm, 24000U, BOARD_TIMER_SOURCE_CLOCK);

    /* Enable channel interrupt flag.*/
    FTM_EnableInterrupts(ftm_instance.FTM_BASEADDR, ftm_instance.FTM_INT);

    /* Enable at the NVIC */
    EnableIRQ(ftm_instance.FTM_IRQ_ID);

    FTM_StartTimer(ftm_instance.FTM_BASEADDR, kFTM_SystemClock);
}

static void InitializeMotorGPIO(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalOutput, 0,
    };

    GPIO_PinInit(BOARD_LEFT_MOTOR_A_GPIO, BOARD_LEFT_MOTOR_A_PIN, &general_config);
    GPIO_PinInit(BOARD_LEFT_MOTOR_B_GPIO, BOARD_LEFT_MOTOR_B_PIN, &general_config);
    GPIO_PinInit(BOARD_LEFT_MOTOR_A_GPIO, BOARD_LEFT_MOTOR_A_PIN, &general_config);
    GPIO_PinInit(BOARD_RIGHT_MOTOR_B_GPIO, BOARD_RIGHT_MOTOR_B_PIN, &general_config);
    GPIO_PinInit(BOARD_MOTOR_EN_GPIO, BOARD_MOTOR_EN_PIN, &general_config);
}

static void InitializeEncoderGPIO(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_ENCODER1_PORT, BOARD_ENCODER1_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_RIGHT_ENCODER_1_PORT, BOARD_RIGHT_ENCODER_1_PIN, kPORT_InterruptFallingEdge);

    EnableIRQ(PORTB_IRQn);
    EnableIRQ(PORTC_IRQn);

    GPIO_PinInit(BOARD_ENCODER1_GPIO, BOARD_ENCODER1_PIN, &general_config);
    GPIO_PinInit(BOARD_ENCODER2_GPIO, BOARD_ENCODER2_PIN, &general_config);

    GPIO_PinInit(BOARD_RIGHT_ENCODER_1_GPIO, BOARD_RIGHT_ENCODER_1_PIN, &general_config);
    GPIO_PinInit(BOARD_RIGHT_ENCODER_2_GPIO, BOARD_RIGHT_ENCODER_2_PIN, &general_config);
}

static void InitializeLimitSwitchGPIO(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_FRONT_LEFT_LIMIT_PORT, BOARD_FRONT_LEFT_LIMIT_PIN, kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(BOARD_FRONT_RIGHT_LIMIT_PORT, BOARD_FRONT_RIGHT_LIMIT_PIN, kPORT_InterruptFallingEdge);

    EnableIRQ(PORTC_IRQn);

    GPIO_PinInit(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN, &general_config);
    GPIO_PinInit(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN, &general_config);
}

void InitializeMotors(void)
{
    LEFT_FTM.FTM_BASEADDR 		= FTM3;
    LEFT_FTM.FTM_CHANNEL 		= kFTM_Chnl_0;
    LEFT_FTM.FTM_INT 			= kFTM_Chnl0InterruptEnable;
    LEFT_FTM.FTM_IRQ_ID 		= FTM3_IRQn;
    LEFT_FTM.FTM_CHANNEL_FLAG 	= kFTM_Chnl0Flag;
    Pwm_Init(LEFT_FTM);
    ftmStructs[eLeftMotor] = LEFT_FTM;

    RIGHT_FTM.FTM_BASEADDR 		= FTM0;
    RIGHT_FTM.FTM_CHANNEL 		= kFTM_Chnl_3;
    RIGHT_FTM.FTM_INT 			= kFTM_Chnl3InterruptEnable;
    RIGHT_FTM.FTM_IRQ_ID 		= FTM0_IRQn;
    RIGHT_FTM.FTM_CHANNEL_FLAG 	= kFTM_Chnl3Flag;
    Pwm_Init(RIGHT_FTM);
    ftmStructs[eRightMotor] = RIGHT_FTM;

    InitializeEncoderGPIO();
    InitializeMotorGPIO();
    InitializeLimitSwitchGPIO();

    UpdateMotorDirection(eLeftMotor, eDirectionReverse);
    UpdateMotorDirection(eRightMotor, eDirectionReverse);
	GPIO_PinWrite(BOARD_MOTOR_EN_GPIO, BOARD_MOTOR_EN_PIN, 1);

	HomeMotors(true);
}

void HomeMotor(motor_t motor, bool wait)
{
	switch(motor){
		case eLeftMotor:
			if(GPIO_PinRead(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN) == 1){
				UpdatePwm(motor, -20);
				if(wait)
					while(GPIO_PinRead(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN) == 1);
			}
			break;
		case eRightMotor:
			if(GPIO_PinRead(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN) == 1){
				UpdatePwm(motor, -20);
				if(wait)
					while(GPIO_PinRead(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN) == 1);
			}
			break;
		default:
			break;
	}
}

void HomeMotors(bool wait)
{
	bool leftAtLimit = GPIO_PinRead(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN) == 0;
	bool rightAtLimit = GPIO_PinRead(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN) == 0;

	/*Make the motors start turning till they hit limit switch*/
	if(!leftAtLimit)
		UpdatePwm(eLeftMotor, -20);
	if(!rightAtLimit)
		UpdatePwm(eRightMotor, -20);

	if(wait){
		/*Limit switch will go low when pressed down, interrupt handler handles stopping the motor
		 * Make sure we wait till we 'zero' the motors at their limtis*/
		while(!leftAtLimit || !rightAtLimit){
			leftAtLimit = GPIO_PinRead(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN) == 0;
			rightAtLimit = GPIO_PinRead(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN) == 0;
		}
	}
}

#define REGISTER_TRUE_MSEC 100
#define REGISTER_FALSE_MSEC 1
void LimitSwitchDebounce(limit_t limitSwitch, uint32_t pollingRate){
	bool rawState = false;
	switch(limitSwitch){
		case eLimitSwitchFrontLeft:
			rawState = GPIO_PinRead(BOARD_FRONT_LEFT_LIMIT_GPIO, BOARD_FRONT_LEFT_LIMIT_PIN) == 0;
			break;
		case eLimitSwitchFrontRight:
			rawState = GPIO_PinRead(BOARD_FRONT_RIGHT_LIMIT_GPIO, BOARD_FRONT_RIGHT_LIMIT_PIN) == 0;
			break;
		default:
			break;
	}
	if(rawState == debouncedLimitState[limitSwitch]){
		/*Set count required for a true to become false when state=true, or a false to become true when state=false*/
		stateCount[limitSwitch] = debouncedLimitState[limitSwitch] ? (REGISTER_FALSE_MSEC / pollingRate) : (REGISTER_TRUE_MSEC / pollingRate);
	}
	else{
		if(--stateCount[limitSwitch] == 0){
			debouncedLimitState[limitSwitch] = rawState;
			stateCount[limitSwitch] = debouncedLimitState[limitSwitch] ? (REGISTER_FALSE_MSEC / pollingRate) : (REGISTER_TRUE_MSEC / pollingRate);
		}
	}
}


void UpdateLimitSwitches(uint32_t pollingRate)
{
	limit_t limitSwitch;

	for(limitSwitch = eLimitSwitchFrontLeft; limitSwitch < eLIMIT_SWITCH_COUNT; limitSwitch++){
		LimitSwitchDebounce(limitSwitch, pollingRate);
	}
}

bool isMotorHome(motor_t motor)
{
	switch(motor){
		case eLeftMotor:
			return debouncedLimitState[eLimitSwitchFrontLeft];
			break;
		case eRightMotor:
			return debouncedLimitState[eLimitSwitchFrontRight];
			break;
		default:
			return false;
	}
}

void UpdatePwm(motor_t motor, int32_t new_pwm)
{
	FTM_Instance_t ftm_instance;
	motor_direction_t direction = g_motorDirection[motor];

	if(new_pwm != 0){
		direction = (new_pwm > 0) ? eDirectionForward : eDirectionReverse;
	}

	new_pwm = abs(new_pwm);
	if(new_pwm >= 100){
		new_pwm = 99;
	}
	else if(new_pwm < 15 && new_pwm != 0){
		new_pwm = 15;
	}

	ftm_instance = ftmStructs[motor];
	g_motorPwm[motor] = new_pwm;

    /* Disable channel output before updating the dutycycle */
    FTM_UpdateChnlEdgeLevelSelect(ftm_instance.FTM_BASEADDR, ftm_instance.FTM_CHANNEL, 0U);

    /* Update PWM duty cycle */
    FTM_UpdatePwmDutycycle(ftm_instance.FTM_BASEADDR, ftm_instance.FTM_CHANNEL, kFTM_CenterAlignedPwm, new_pwm);

    /* Software trigger to update registers */
    FTM_SetSoftwareTrigger(ftm_instance.FTM_BASEADDR, true);

    /* Start channel output with updated dutycycle */
    FTM_UpdateChnlEdgeLevelSelect(ftm_instance.FTM_BASEADDR, ftm_instance.FTM_CHANNEL, kFTM_HighTrue);

    if(new_pwm != 0)
    	UpdateMotorDirection(motor, direction);
}


void UpdateMotorDirection(motor_t motor, motor_direction_t direction)
{
	GPIO_Type * gpio_base_a;
	GPIO_Type * gpio_base_b;
	uint32_t gpio_pin_a, gpio_pin_b;
	switch(motor)
	{
		case eLeftMotor:
			if(direction == g_motorDirection[eLeftMotor]) return;
			gpio_base_a = BOARD_LEFT_MOTOR_A_GPIO;
			gpio_base_b = BOARD_LEFT_MOTOR_B_GPIO;
			gpio_pin_a = BOARD_LEFT_MOTOR_A_PIN;
			gpio_pin_b = BOARD_LEFT_MOTOR_B_PIN;
			g_motorDirection[eLeftMotor] = direction;
			break;
		case eRightMotor:
			if(direction == g_motorDirection[eRightMotor]) return;
			//Right has a's and b's switched as it is mounted mirrored
			gpio_base_a = BOARD_RIGHT_MOTOR_B_GPIO;
			gpio_base_b = BOARD_RIGHT_MOTOR_A_GPIO;
			gpio_pin_a = BOARD_RIGHT_MOTOR_B_PIN;
			gpio_pin_b = BOARD_RIGHT_MOTOR_A_PIN;
			g_motorDirection[eRightMotor] = direction;
			break;
		default:
			return;
	}

	switch(direction)
	{
		case eDirectionForward:
		    GPIO_PinWrite(gpio_base_a, gpio_pin_a, 1);
			GPIO_PinWrite(gpio_base_b, gpio_pin_b, 0);
			break;
		case eDirectionReverse:
		    GPIO_PinWrite(gpio_base_a, gpio_pin_a, 0);
			GPIO_PinWrite(gpio_base_b, gpio_pin_b, 1);
			break;
		default:
			break;
	}
}

void stopMotor(motor_t motor)
{
	UpdatePwm(motor, 0);
}

void turnDegrees(motor_t motor, int32_t degrees, uint16_t pwm)
{
	int64_t finalPulseCount = pulseCount[motor] + (degrees * 396)/360; //Multiply then divide to preserve as much integer math as possible
	UpdatePwm(motor, pwm);
	while(pulseCount[motor] != finalPulseCount);
	stopMotor(motor);
}

void turnDegreesPID(motor_t motor, int32_t degrees)
{
	int64_t finalPulseCount = pulseCount[motor] + (degrees * 396)/360; //Multiply then divide to preserve as much integer math as possible
	int64_t error = 0;
	double integral = 0;

	double Kp = 0.5;
	double Ki = 0.01;
	int32_t newPwm;
	while(1){
		error 	= finalPulseCount - pulseCount[motor];
		integral = integral + error*0.0001;
		newPwm 	= (int32_t)(Kp*error + Ki*integral);
		UpdatePwm(motor, newPwm);

		if(error == 0){
			stopMotor(motor);
			break;
		}
	}
}

void goToDegree(motor_t motor, int32_t degree)
{
	int64_t finalPulseCount =  (degree * 396)/360;
	int64_t error = finalPulseCount - pulseCount[motor];
	if(abs(error) < 10){
		return;
	}
	int32_t pwm = (error > 0) ? 30 : -30;
	UpdatePwm(motor, pwm);
	while(pulseCount[motor] != finalPulseCount);
	stopMotor(motor);
}

void goToDegreePID(motor_t motor, int32_t degree)
{
	int64_t finalPulseCount =  (degree * 396)/360;
	int64_t error = finalPulseCount - pulseCount[motor];

	double integral = 0;
	double derivative = 0;
	double Kp = 10.3256754613097;
	double Ki = 10.5401739097128;
	double Kd = 0.106445159509409;
	double Ts = 0.0002;
	int32_t newPwm;

	error 	= finalPulseCount - pulseCount[motor];
	integral = integral + error*Ts;
	derivative = (error-previousError[motor]) / Ts;
	newPwm 	= (int32_t)(Kp*error + Ki*integral + Kd*derivative);
	UpdatePwm(motor, newPwm);

	previousError[motor] = error;
}
