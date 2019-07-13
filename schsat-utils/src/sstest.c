#include "../../libschsat.h"
#include <stdio.h>
#include <stdint.h>

uint32_t reverseBits(register uint32_t x)
{
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
    return((x >> 16) | (x << 16));
}
void control(){
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
int main(){
	control();
}
