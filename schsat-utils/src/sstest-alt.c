/*#include "../../llibschsat.h"
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
		uint8_t data1[6];
		uint8_t data2[6];
		uint8_t data3[6];
		uint32_t vect_x = 0, vect_y = 0, vect_z = 0;
		recieve_unican_message(0x10, 0xB21, data1, 6, 1000);
		vect_x = reverseBits(((data1[0] << 24) | (data1[1] << 16) | (data1[2] << 8) | (data1[3])));
		recieve_unican_message(0x10, 0xB22, data2, 6, 1000);
		vect_y = reverseBits(((data2[0] << 24) | (data2[1] << 16) | (data2[2] << 8) | (data2[3])));
		recieve_unican_message(0x10, 0xB23, data3, 6, 1000);
		vect_z = reverseBits(((data3[0] << 24) | (data3[1] << 16) | (data3[2] << 8) | (data3[3])));
		mSleep(100);
		printf("Sun sensor raw values: %d %d %d\r\n", vect_x, vect_y, vect_z);
	}
	return;
}

//int main(){
//	control();
//}
*/
