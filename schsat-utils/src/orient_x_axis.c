#include "libschsat.h"

#define mlrd 1000000000.0

void set_euler_angles(float x, float y, float z)
{
	float XYZ[3] = {x, y, z};
	send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
}

void control()
{
	puts("Trying to rotate on 0;0;0 Euler angles");
	Sleep(10);
	uint64_t dataX = 0, dataY = 0, dataZ = 0;
	float vecRawX = 0, vecRawY = 0, vecRawZ = 0;
	float vecX[72][2] = {0}, vecY[72][2] = {0}, vecZ[72][2] = {0};
	float tempX = 0, tempY = 0, tempZ = 0;
	uint8_t actualX = 0, actualY = 0, actualZ = 0;
	float angle = 0;
	for (int i = 0; i < 72; i++)
	{
		recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		recieve_unican_message(0x10, 0xB22, (uint8_t*)(&dataY), 48, 1000);
		recieve_unican_message(0x10, 0xB23, (uint8_t*)(&dataZ), 48, 1000);
		vecRawX = dataX >> 16; vecRawY = dataY >> 16; vecRawZ = dataZ >> 16;
		dataX = dataX << 32; dataY = dataY << 32; dataZ = dataZ << 32;
		tempX = dataX >> 8; tempY = dataY >> 8; tempZ = dataZ >> 8;
		actualX = dataX << 8; actualY = dataY << 8; actualZ = dataZ << 8;
		vecX[i][0] = vecRawX / mlrd; vecY[i][0] = vecRawY / mlrd; vecZ[i][0] = vecRawZ / mlrd;
		vecX[i][1] = angle; vecY[i][1] = angle; vecZ[i][1] = angle;
		puts("\nAngle %f / 360.\n");
		printf("Vector X: %f. Vector Y: %f. Vector Z: %f\n", vecX[i][0], vecY[i][0], vecZ[i][0]);
		printf("Temp X: %f. Temp Y: %f. Temp Z: %f\n", tempX, tempY, tempZ);
		printf("Actual?: X: %d. Y: %d. Z: %d.", actualX, actualY, actualZ);
		puts("\n************\n");
		angle += 5.0;
		set_euler_angles(angle, 0, 0);
		Sleep(10);
	}
	freopen ("data.csv", "w" , stdout);
	printf ("Vector X,Vector Y,Vector Z");
	float maxAngleX = 0, maxAngleY = 0, maxAngleZ = 0;
	float maxVectorX = vecX[0][0], maxVectorY = vecY[0][0], maxVectorZ = vecZ[0][0];
	for (int i = 0; i < 72; i++)
	{
		printf("%f,%f,%f\n", vecX[i][0], vecY[i][0], vecZ[i][0]);
		if (vecX[i][0] > maxVectorX)
		{
			maxVectorX = vecX[i][0];
			maxAngleX = vecX[i][1];
		}
		if (vecY[i][0] > maxVectorY)
		{
			maxVectorY = vecY[i][0];
			maxAngleY = vecY[i][1];
		}
		if (vecZ[i][0] > maxVectorZ)
		{
			maxVectorZ = vecZ[i][0];
			maxAngleZ = vecZ[i][1];
		}
	}
	printf("\n%f,%f,%f\n%f,%f,%f", maxVectorX, maxVectorY, maxVectorZ, maxAngleX, maxAngleY, maxAngleZ);
	fclose(stdout);
	printf("Max angles was:\nAngle X: %f. Angle Y: %f. Angle Z: %f\n", maxAngleX, maxAngleY, maxAngleZ);
	printf("With vectors:\nVector X: %f. Vector Y: %f. Vector Z: %f.\n", maxVectorX, maxVectorY, maxVectorZ);
	printf("Rotating at %f;0;0 Euler angles...", maxAngleX);
}