#ifndef _SHCSATUTILS_H
#define _SHCSATUTILS_H

#include <stdio.h>
#include <stdint.h>

void calibrateGyroscope();
uint32_t reverseBits(register uint32_t x);
void readSunSensor();
void readSunSensor_alt();

#endif
