#include "libschsat.h"
#include <stdio.h>
#include <stdint.h>

void control(){
	puts("Init ver scetch\n");
	uint8_t data[3];
	send_unican_message(0x05, 0xFFE0, NULL, 0);
	recieve_unican_message(0x05, 0xFFE1, data, 3, 100);
	printf("MTCU onboard software version: %d.%d.%d", data[0], data[1], data[2]);
	send_unican_message(0x19, 0xFFE0, NULL, 0);
	recieve_unican_message(0x19, 0xFFE1, data, 3, 100);
	printf("Carrier onboard software version: %d.%d.%d", data[0], data[1], data[2]);
}