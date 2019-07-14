#include "libschsat.h"

#define mlrd 1000000000.0

void set_euler_angles(float x, float y, float z)
{
	float XYZ[3] = {x, y, z};
	send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
}

void orient_on_sun()
{
	uint64_t dataX = 0, dataY = 0, dataZ = 0;
	uint32_t vecX = 0, vecY = 0, vecZ = 0;
	uint8_t tempX = 0, tempY = 0, tempZ = 0;
	float toShowX = 0, toShowY = 0, toShowZ = 0;
	uint32_t vecXA[72][2], vecYA[72][2], vecZA[72][2];
	set_euler_angles(0, 0, 0);
	puts("Trying to orient on 0 0 0...");
	Sleep(5);
	int angle = 0;
	for (int i = 0; i < 72; i++)
	{
		angle += 5;
		set_euler_angles(angle, 0, 0);
		recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		recieve_unican_message(0x10, 0xB22, (uint8_t*)(&dataY), 48, 1000);
		recieve_unican_message(0x10, 0xB23, (uint8_t*)(&dataZ), 48, 1000);
		vecX = dataX >> 16; vecY = dataY >> 16; vecZ = dataZ >> 16;
		dataX = dataX << 32; dataY = dataY << 32; dataZ = dataZ << 32;
		tempX = dataX >> 8; tempY = dataY >> 8; tempZ = dataZ >> 8;
		dataX = dataX << 8; dataY = dataY << 8; dataZ = dataZ << 8;
		toShowX = vecX / mlrd; toShowY = vecY / mlrd; toShowZ = vecZ / mlrd;
		printf("Angle: %d / 360. \nVector X: %f ; Vector Y: %f ; Vector Z: %f ;\nTemp X: %d ; Temp Y: %d ; Temp Z: %d ;\n\n *************************************** \n\n", angle, toShowX, toShowY, toShowZ, tempX, tempY, tempZ);
		vecXA[i][0] = vecX; vecYA[i][0] = vecY; vecZA[i][0] = vecZ;
		vecXA[i][1] = angle; vecYA[i][1] = angle; vecZA[i][1] = angle;
		Sleep(2);
	}
	int maxX = vecXA[0][0], maxY = vecYA[0][0], maxZ = vecZA[0][0];
	int maxAngleX = 0, maxAngleY = 0, maxAngleZ = 0;
	for (int i = 0; i < 72; i++)
	{
		if (vecXA[i][0] > maxX)
		{
			maxX = vecXA[i][0];
			maxAngleX = vecXA[i][1];
		}
		if (vecYA[i][0] > maxY)
		{
			maxY = vecYA[i][0];
			maxAngleY = vecYA[i][1];
		}
		if (vecZA[i][0] > maxZ)
		{
			maxZ = vecZA[i][0];
			maxAngleZ = vecZA[i][1];
		}
	}
	
	float toShowMaxX = maxX / mlrd, toShowMaxY = maxY / mlrd, toShowMaxZ = maxZ / mlrd;
	printf("Maximal vectors was: \nX: %f ; Y: %f ; Z: %f ;\nAt angles: \nX: %d ; Y: %d ; Z: %d ;\n", toShowMaxX, toShowMaxY, toShowMaxZ, maxAngleX, maxAngleY, maxAngleZ);
	puts("Trying to orient on sun...");
	set_euler_angles(maxAngleX, 0.0, 0.0);
}

void calibrate_gyro()
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
	if (abs(newAvgX) >= 1.5 || abs(newAvgY) >= 1.5 || abs(newAvgZ) >= 1.5)
	{
		printf("Average values are too long away from 0, calibrating again.");
		calibrate_gyro();
	}
	puts("Calibrated!");
}

void control()
{
	calibrate_gyro();
	//orient_on_sun();
}