#include "accelerometer.h"

MPU6050 mpu;

Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

#define _BV(bit) \
	(1 << (bit))

bool InitializeAccelerometer(void)
{
    gpio_pin_config_t general_config = {
        kGPIO_DigitalInput, 0,
    };

    PORT_SetPinInterruptConfig(BOARD_MPU6050_INTERRUPT_PORT, BOARD_MPU6050_INTERRUPT_PIN, kPORT_InterruptRisingEdge);
    EnableIRQ(PORTB_IRQn);
    GPIO_PinInit(BOARD_MPU6050_INTERRUPT_GPIO, BOARD_MPU6050_INTERRUPT_PIN, &general_config);

    //Uncomment this if you want to test the i2c line
    //TestMasterTransmit();

	mpu.initialize();
	bool isProperlyConnected = mpu.testConnection();
	if(!isProperlyConnected){
		return false;
	}
	devStatus = mpu.dmpInitialize();
	if(devStatus == 0){
		SetOffsets();
		mpu.setDMPEnabled(true);
		mpuIntStatus = mpu.getIntStatus();
		dmpReady = true;
		packetSize = mpu.dmpGetFIFOPacketSize();
		return true;
	}
	return false;
}

//Offsets determined by values calibrated with arduino
void SetOffsets(void)
{
    mpu.setXGyroOffset(-506);
    mpu.setYGyroOffset(-1);
    mpu.setZGyroOffset(15);
    mpu.setZAccelOffset(1157);
    mpu.setXAccelOffset(475);
    mpu.setYAccelOffset(-2003);
}

void UpdateMotion6Values()
{
	// reset interrupt flag and get INT_STATUS byte
	mpuIntStatus = mpu.getIntStatus();

	// get current FIFO count
	fifoCount = mpu.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
		// reset so we can continue cleanly
		mpu.resetFIFO();
		fifoCount = mpu.getFIFOCount();
	}
	else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);

		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		//Get Readable Real-World Accel, Adjusted to remove gravity
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetAccel(&aa, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
	}
}

void GetMotionValues(float * ax, float * ay, float * az, float * yaw, float * pitch, float * roll)
{
	if(bUpdateAccelerometer){
		UpdateMotion6Values();
		bUpdateAccelerometer = false;
	}
	*ax = aaReal.x;
	*ay = aaReal.y;
	*az = aaReal.z;
	*yaw = ypr[0];
	*pitch = ypr[1];
	*roll = ypr[2];
}
