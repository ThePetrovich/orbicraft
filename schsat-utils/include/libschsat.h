/* This file was automatically generated.  Do not edit! */
#ifndef LIBSCHSAT_H_
#define LIBSCHSAT_H_
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#define LSS_OK 0
#define LSS_ERROR 1
#define LSS_BREAK 2
int camera_take_photo(uint16_t num);
int gyro_push_data_uhf();
int gyro_request_raw(uint16_t num,float *pRAW_dataX,float *pRAW_dataY,float *pRAW_dataZ);
int gyro_set_offset(uint16_t num,float offset_X,float offset_Y,float offset_Z);
int magnetometer_push_data_uhf();
int magnetometer_request_raw(uint16_t num,float *pRAW_dataX,float *pRAW_dataY,float *pRAW_dataZ);
int magnetometer_set_offset(uint16_t num,int16_t offset_X,int16_t offset_Y,int16_t offset_Z);
int motor_request_speed(uint16_t num,float *pRPM);
int motor_set_speed(uint16_t num,float RPM);
int recieve_unican_message(const uint16_t address_from,const uint16_t msg_id,uint8_t *data,const uint16_t msg_size,const uint16_t timeout);
int send_unican_message(const uint16_t address_to,const uint16_t msg_id,uint8_t *data,const uint16_t msg_size);
int sunsensor_get_raw(uint16_t num,int32_t *angle);
int sunsensor_push_data_uhf(uint16_t num);
int sunsensors_angle(uint16_t positive,uint16_t negative,uint16_t ambient,double *angle);
int transceiver_request_buff(uint16_t num,uint8_t *data);
int transceiver_request_reset(uint16_t num);
int transceiver_send(uint16_t tx_num,uint16_t rx_num,const uint8_t *data,uint16_t len);
int transmitter_transmit_photo(uint16_t num);
void dump_buffer(uint8_t *buf,size_t len);
void mSleep(int msec);
void Sleep(int sec);
#endif
