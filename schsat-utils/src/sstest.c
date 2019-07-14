#include "../include/schsat-utils.h"

void readSunSensor(){
	puts("Reading sun sensor values");
	while(1){
	/*	uint8_t data1[6];
		uint8_t data2[6];
		uint8_t data3[6];*/
		uint8_t sensorData_period = 100;
		send_unican_message(0x10, 0x23, &sensorData_period, 1);
		int32_t vect_x = 0, vect_y = 0, vect_z = 0;
		recieve_unican_message(0x10, 0xB21, (uint8_t *)&vect_x, 4, 100);
		recieve_unican_message(0x10, 0xB22, (uint8_t *)&vect_y, 4, 100);
		recieve_unican_message(0x10, 0xB23, (uint8_t *)&vect_z, 4, 100);
		printf("Sun sensor raw values (not reversed): %f %f %f\r\n", (double)vect_x/1000000000.0, (double)vect_y/1000000000.0, (double)vect_z/1000000000.0);
		//vect_x = reverseBits(vect_x);
		//vect_y = reverseBits(vect_y);
		//vect_z = reverseBits(vect_z);
		//printf("Sun sensor raw values (reversed): %f %f %f\r\n", (double)vect_x/1000000000.0, (double)vect_y/1000000000.0, (double)vect_z/1000000000.0);
		mSleep(50);
	}
	return;
}
