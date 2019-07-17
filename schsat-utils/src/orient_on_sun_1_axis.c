#include "libschsat.h"

#define mlrd 1000000000.0

void set_euler_angles(float x, float y, float z)
{
	float XYZ[3] = {x, y, z};
	send_unican_message(0x5, 0xF010, (uint8_t*)(XYZ), 96);
}

void control()
{
	puts("Rotating at 0;0;0 Euler angles, please wait 20 sec...");
	set_euler_angles(0,0,0);
	Sleep(20);
	float curData[6] = {0};
	int it = 0, rtd = 0;
	uint32_t rawVecX = 0;
	float vecX[1000][2] = {0};
	float curAngleX = 0, curAngleY = 0, curAngleZ = 0;
	uint64_t dataX = 0;
	set_euler_angles(120,0,0);
	while (rtd != 1)
	{
		int status = recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		rawVecX = dataX >> 16; 
		int status2 = recieve_unican_message(0x5, 0xF201, (uint8_t*)(curData), 192, 1000);
		curAngleX = curData[0]; curAngleY = curData[1]; curAngleZ = curData[2];
		vecX[it][0] = rawVecX / mlrd;
		vecX[it][1] = curAngleX;
		printf("Status vector: %d ; Status angles: %d \n", status, status2);
		printf("Current angles: X: %f ; Y: %f ; Z: %f\n", curAngleX, curAngleY, curAngleZ);
		printf("Current vector X: %f\n************\n", vecX[it][0]);
		
		if (curAngleX > 119 && curAngleX < 121)
			rtd = 1;
		Sleep(1);
		it++;
	}
	rtd = 0;
	puts("Rotated at 120! Rotating at -120...");
	set_euler_angles(-120,0,0);
	while (rtd != 1)
	{
		int status = recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		rawVecX = dataX >> 16; 
		int status2 = recieve_unican_message(0x5, 0xF201, (uint8_t*)(curData), 192, 1000);
		curAngleX = curData[0]; curAngleY = curData[1]; curAngleZ = curData[2];
		vecX[it][0] = rawVecX / mlrd;
		vecX[it][1] = curAngleX;
		printf("Status vector: %d ; Status angles: %d \n", status, status2);
		printf("Current angles: X: %f ; Y: %f ; Z: %f with iterator %d\n", curAngleX, curAngleY, curAngleZ, it);
		printf("Current vector X: %f\n************\n", vecX[it][0]);
		
		if (curAngleX > -121 && curAngleX < -119)
			rtd = 1;
		Sleep(1);
		it++;
	}
	rtd = 0;
	puts("Rotated at -120! Rotating at 0...");
	set_euler_angles(0,0,0);
	while (rtd != 1)
	{
		int status = recieve_unican_message(0x10, 0xB21, (uint8_t*)(&dataX), 48, 1000);
		rawVecX = dataX >> 16;
		int status2 = recieve_unican_message(0x5, 0xF201, (uint8_t*)(curData), 192, 1000);
		curAngleX = curData[0]; curAngleY = curData[1]; curAngleZ = curData[2];
		vecX[it][0] = rawVecX / mlrd;
		vecX[it][1] = curAngleX;
		printf("Status vector: %d ; Status angles: %d \n", status, status2);
		printf("Current angles: X: %f ; Y: %f ; Z: %f with iterator %d\n", curAngleX, curAngleY, curAngleZ, it);
		printf("Current vector X: %f\n************\n", vecX[it][0]);
		
		if (curAngleX > -121 && curAngleX < -119)
			rtd = 1;
		Sleep(1);
		it++;
	}
	puts("Rotated at 0 back. Calculating...");
	float maxAngleX = vecX[0][1], maxVectorX = vecX[0][0];
	for (int i = 0; i < 1000; i++)
	{
		if (vecX[i][0] > maxVectorX)
		{
			maxVectorX = vecX[i][0];
			maxAngleX = vecX[i][1];
		}
	}
	printf("Best X angle was %f with vector %f\n", maxAngleX, maxVectorX);
	printf("Rotating at %f;0;0 Euler angles...", maxAngleX);
	set_euler_angles(maxAngleX,0,0);
}