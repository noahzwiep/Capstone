#include "gps.h"

uint8_t UPDATE_10HZ[] = PMTK_SET_NMEA_UPDATE_10HZ;
uint8_t UPDATE_ONLY_RMC[] = PMTK_SET_NMEA_OUTPUT_RMCONLY;
uint8_t UPDATE_POSITION_10HZ[] = PMTK_API_SET_FIX_CTL_5HZ;

float currentKnots = 0;

bool InitializeGPS(void)
{
	//Pins and interrupts are initialized in BOARD_InitPeripherals(void)

	//PMTK_SET_NMEA_OUTPUT_RMCONLY
	UART_WriteBlocking(UART3_PERIPHERAL, UPDATE_ONLY_RMC, sizeof(UPDATE_ONLY_RMC)/sizeof(UPDATE_ONLY_RMC[0]) -1);

	//PMTK_API_SET_FIX_CTL_5HZ
	UART_WriteBlocking(UART3_PERIPHERAL, UPDATE_POSITION_10HZ, sizeof(UPDATE_POSITION_10HZ)/sizeof(UPDATE_POSITION_10HZ[0]) -1);

	//PMTK_SET_NMEA_UPDATE_10HZ
	UART_WriteBlocking(UART3_PERIPHERAL, UPDATE_10HZ, sizeof(UPDATE_10HZ)/sizeof(UPDATE_10HZ[0]) -1);

	return true;
}

double getGpsVelocity(void)
{
	int i = 0;

	if(bCheckGps){
		char *ptr = strtok(gpsRxBuffer, ",");
		while(ptr != NULL)
		{
			if(i == 7){
				currentKnots = atof(ptr);
				break;
			}
			ptr = strtok(NULL, ",");
			i++;
		}
		bCheckGps = false;
	}

	/*for(int i = 0; i<GPS_BUFFER_SIZE; i++)
	{
		printf("%c", gpsRxBuffer[i]);
	}
	printf("\n");

	printf("%f\n", knots);
	*/
	//Knots to m/s
	return currentKnots*0.514444;
}
