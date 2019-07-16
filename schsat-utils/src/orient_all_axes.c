#include "libschsat.h"

#define mlrd 1000000000.0

void set_euler_angles(float x, float y, float z)
{
	float XYZ[3] = {x, y, z};
	send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
}

void control()
{
	printf("Rotating on 0;0;0 Euler angles...");
	set_euler_angles(0,0,0);
	Sleep(10);
	
	printf("Rotating by X axis...");
	int angleX = 0;
	uint64_t dataX = 0;
	float rawVecX = 0.0, tempX = 0.0, vecX[72][2] = {0};
	uint8_t actualX;
	for (int i = 0; i < 72 ; i++)
	{
		recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		rawVecX = dataX >> 16; 
		dataX = dataX << 32; 
		tempX = dataX >> 8; 
		actualX = dataX << 8;
		vecX[i][0] = rawVecX / mlrd;
		printf("Angle: %d / 360\nVector X: %f ; Temperature X: %f ; Is it Actual?: %d\n******\n", angleX, vecX[i][0], tempX, actualX);
		vecX[i][1] = angleX;
		angleX += 5;
		set_euler_angles(angleX, 0, 0); 
		Sleep(10);
	}
	
	printf("Rotating by Y axis...");
	int angleY = 0;
	uint64_t dataY = 0;
	float rawVecY = 0.0, tempY = 0.0, vecY[72][2] = {0};
	uint8_t actualY;
	for (int i = 0; i < 72 ; i++)
	{
		recieve_unican_message(0x10, 0xB22, (uint8_t*)(&dataY), 48, 1000);
		rawVecY = dataY >> 16; 
		dataY = dataY << 32; 
		tempY = dataY >> 8; 
		actualY = dataY << 8;
		vecY[i][0] = rawVecY / mlrd;
		printf("Angle: %d / 360\nVector Y: %f ; Temperature Y: %f ; Is it Actual?: %d\n******\n", angleY, vecY[i][0], tempY, actualY);
		vecY[i][1] = angleY;
		angleY += 5;
		set_euler_angles(0, angleY, 0); 
		Sleep(10);
	}
	
	printf("Rotating by Z axis...");
	int angleZ = 0;
	uint64_t dataZ = 0;
	float rawVecZ = 0.0, tempZ = 0.0, vecZ[72][2] = {0};
	uint8_t actualZ;
	for (int i = 0; i < 72 ; i++)
	{
		recieve_unican_message(0x10, 0xB23, (uint8_t*)(&dataZ), 48, 1000);
		rawVecZ = dataZ >> 16; 
		dataZ = dataZ << 32; 
		tempZ = dataZ >> 8; 
		actualZ = dataZ << 8;
		vecZ[i][0] = rawVecZ / mlrd;
		printf("Angle: %d / 360\nVector Z: %f ; Temperature Z: %f ; Is it Actual?: %d\n******\n", angleZ, vecZ[i][0], tempZ, actualZ);
		vecZ[i][1] = angleZ;
		angleZ += 5;
		set_euler_angles(0, 0, angleZ); 
		Sleep(10);
	}
	
	float maxVecX = vecX[0][0], maxVecY = vecY[0][0], maxVecZ = vecZ[0][0];
	float maxAngleX = vecX[0][1], maxAngleY = vecY[0][1], maxAngleZ = vecZ[0][1];
	for (int i = 0; i < 72; i++)
	{
		if (vecX[i][0] > maxVecX)
		{
			maxVecX = vecX[i][0];
			maxAngleX = vecX[i][1];
		}
		if (vecY[i][0] > maxVecY)
		{
			maxVecY = vecY[i][0];
			maxAngleY = vecY[i][1];
		}
		if (vecY[i][0] > maxVecX)
		{
			maxVecY = vecY[i][0];
			maxAngleY = vecY[i][1];
		}
	}
	
	printf("Maximal angles was\nX: %d with vector %f\nY: %d with vector %f\nZ: %d with vector %f\n******\n", maxAngleX, maxVecX, maxAngleY, maxVecY, maxAngleZ, maxVecZ);
	printf("Rotating on %d;%d;%d Euler angles...\n", maxAngleX, maxAngleY, maxAngleZ);
	puts("You have 5 seconds to check the angles");
	Sleep(5);
	set_euler_angles(maxAngleX, maxAngleY, maxAngleZ);
}

