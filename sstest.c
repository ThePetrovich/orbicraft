#include "libschsat.h"

uint64_t dataX, dataY, dataZ;
uint32_t vecX, vecY, vecZ;
uint8_t tempX, tempY, tempZ;
bool actualX, actualY, actualZ;
uint32_t vecXA[72], vecYA[72], vecZA[72];
void control()
{
	for (i = 0; i < 360; i += 5)
	{
		recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		vecX = dataX >> 16; vecY = dataY >> 16; vecZ = dataZ >> 16;
		dataX = dataX << 32; dataY = dataY << 32; dataZ = dataZ << 32;
		tempX = dataX >> 8; tempY = dataY >> 8; tempZ = dataZ >> 8;
		dataX = dataX << 8; dataY = dataY << 8; dataZ = dataZ << 8;
		actualX = dataX >> 7; actualY = dataY >> 7; actualZ = dataZ >> 7;
		printf("Iterator: %d / 360. \n Vector X: %d ; Vector Y: %d ; Vector Z: %d ;\nTemp X: %d ; Temp Y: %d ; Temp Z: %d ;\n\n *************************************** \n\n", i, vecX, vecY, vecZ, tempX, tempY, tempZ);
		vecXA[i] = vecX; vecYA[i] = vecY; vecZA[i] = vecZ;
	}
	int maxX = vecXA[0], maxY = vecYA[0], maxZ = vecZA[0];
	for (i = 0; i < 72; i++)
	{
		if (vecXA[i] > maxX)
			maxX = vecXA[i];
		if (vecYA[i] > maxY)
			maxY = vecYA[i];
		if (vecZA[i] > maxZ)
			maxZ = vecZA[i];
	}

	printf("Maximal vectors was: %d %d %d", maxX, maxY, maxZ);
}