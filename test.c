#include "libschsat.h"
#include <stdio.h>
#include <stdint.h>

void control(){
	puts("Hello world\r\n");
	uint8_t data[3] = {1, 1, 1};
	uint8_t i = 0;
	send_unican_message(0x00, 0x0, NULL, 0);
	send_unican_message(0x05, 0xFFE0, NULL, 0);
	//i++;
	recieve_unican_message(0x05, 0xFFE1, data, 3, 1000);
	//i++;
	printf("MTCU onboard software version: %d %d %d\n", data[0], data[1], data[2]);
	send_unican_message(0x19, 0xFFE0, NULL, 0);
	//i++;
	recieve_unican_message(0x19, 0xFFE1, data, 3, 1000);
	//i++;
	printf("Carrier onboard software version: %d %d %d %d\n", data[0], data[1], data[2], i);
	return;
}