#include "libschsat.h"

#define mlrd 1000000000
uint64_t dataX, dataY, dataZ;
uint32_t vecX, vecY, vecZ;
uint8_t tempX, tempY, tempZ;
uint32_t vecXA[72][2], vecYA[72][2], vecZA[72][2];
float XYZ[3];
float angle = 0.0;
void control()
{
	for (int i = 0; i < 72; i++)
	{
		angle += 5;
		XYZ[0] = angle;
		XYZ[1] = 0;
		XYZ[2] = 0;
		send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
		recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		vecX = dataX >> 16; vecY = dataY >> 16; vecZ = dataZ >> 16;
		dataX = dataX << 32; dataY = dataY << 32; dataZ = dataZ << 32;
		tempX = dataX >> 8; tempY = dataY >> 8; tempZ = dataZ >> 8;
		dataX = dataX << 8; dataY = dataY << 8; dataZ = dataZ << 8;
		vecX /= mlrd; vecY /= mlrd; vecZ /= mlrd;
		printf("Angle: %d / 360. \n Vector X: %d ; Vector Y: %d ; Vector Z: %d ;\nTemp X: %d ; Temp Y: %d ; Temp Z: %d ;\n\n *************************************** \n\n", i, vecX, vecY, vecZ, tempX, tempY, tempZ);
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

	printf("Maximal vectors was: \nX: %d ; Y: %d ; Z: %d ;\nAt angles: \nX: %d ; Y: %d ; Z: %d ;\n", maxX, maxY, maxZ, maxAngleX, maxAngleY, maxAngleZ);
	puts("Trying to orient on sun...");
	float oXYZ[3] = {maxAngleX, 0.0, 0.0};
	send_unican_message(0x5, 0xF010, (uint8_t*)(oXYZ), 96);
}