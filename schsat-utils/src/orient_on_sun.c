#include "libschsat.h"

#define mlrd 1000000000.0

void set_euler_angles(float x, float y, float z)
{
	float XYZ[3] = {x, y, z};
	send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
}

void control()
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