#include "libschsat.h"

void control()
{
float dataX[2] = {0}, dataY[2] = {0}, dataZ[2] = {0};
	float sumX = 0, sumY = 0, sumZ = 0;
	puts ("Calibrating, estimated time: ~100 sec...");
	for (int i = 0; i < 100; i++)
	{	
		if ((gyro_request_raw(0, &dataX[1], &dataY[1], &dataZ[1])) == 0)
		{
			printf("X: %f ; Y: %f ; Z: %f\n", dataX[1], dataY[1], dataZ[1]);
			if (i > 1)
			{ 
				if (abs(dataX[1] - dataX[0]) >= 5)
				{
					dataX[1] = dataX[0];
					printf ("Delta X was bigger than 5, now X = %f\n", dataX[0]);
				}
				if (abs(dataY[1] - dataY[0]) >= 5)
				{
					dataY[1] = dataY[0];
					printf ("Delta Y was bigger than 5, now Y = %f\n", dataY[0]);
				}
				if (abs(dataZ[1] - dataZ[0]) >= 5)
				{
					dataZ[1] = dataZ[0];
					printf ("Delta Z was bigger than 5, now Z = %f\n", dataZ[0]);
				}
			}
			printf("\n ********************* \n");
			sumX += dataX[1];
			sumY += dataY[1];
			sumZ += dataZ[1];
			dataX[0] = dataX[1];
			dataY[0] = dataY[1];
			dataZ[0] = dataZ[1];
		}
		else
		{
			puts("Error while reading gyro data, trying again...");
		}
		Sleep(1);
	}
	float avgX = sumX / 100.0, avgY = sumY / 100.0, avgZ = sumZ / 100.0;
	printf ("Average values was: X: %f ; Y: %f ; Z: %f\n", avgX, avgY, avgZ);
	Sleep(5);
	float refs[3] = {0.0};
	send_unican_message(0x19, 0X0BE8, NULL, 0);
	recieve_unican_message(0x19, 0x0BE1, (uint8_t*)(refs), 96, 1000);
	float refX = refs[0];
	float refY = refs[1];
	float refZ = refs[2];
	
	printf("Current offset: X: %f ; Y: %f ; Z: %f ;\nSet offset: X: %f ; Y: %f ; Z: %f\nYou have 5 seconds to check values\n", refX, refY, refZ, avgX + refX, avgY + refY, avgZ + refZ);
	Sleep(5);
	gyro_set_offset(0, avgX + refX, avgY + refY, avgZ + refZ);
	puts("Checking current offset value, estimated time: ~100 sec...");
	float newDataX = 0, newDataY = 0, newDataZ = 0;
	float newSumX = 0, newSumY = 0, newSumZ = 0;
	for(int i = 0 ; i < 100; i++)
	{
		gyro_request_raw(0, &newDataX, &newDataY, &newDataZ);
		printf("X: %f ; Y: %f ; Z: %f ; \n" , newDataX, newDataY, newDataZ);
		newSumX += newDataX;
		newSumY += newDataY;
		newSumZ += newDataZ;
		Sleep(1);
	}
	float newAvgX = newSumX / 100.0, newAvgY = newSumY / 100.0, newAvgZ = newSumZ / 100.0;
	printf("Current average values is: X: %f ; Y: %f ; Z: %f \n", newAvgX, newAvgY, newAvgZ);
	if (abs(newAvgX) >= 0.1 || abs(newAvgY) >= 0.1 || abs(newAvgZ) >= 0.1)
	{
		printf("Average values are too long away from 0, calibrating again.");
		control();
	}
	puts("Calibrated!");
}
