 /*#include "../../llibschsat.h"

void calibrateGyroscope(){
    float RAW_dataX, RAW_dataY, RAW_dataZ, counter_x, counter_y, counter_z;
    float RAW_dataX_p, RAW_dataY_p, RAW_dataZ_p;
    float data_sum_x = 0, data_sum_y = 0, data_sum_z = 0, data_avg_x, data_avg_y, data_avg_z;

    gyro_request_raw(0, &RAW_dataX, &RAW_dataY, &RAW_dataZ);
    data_avg_x = RAW_dataX;
    data_avg_y = RAW_dataY;
    data_avg_z = RAW_dataZ;

    printf("Gyroscope calibration\n");

    for(int i = 0; i < 500; i++){
        gyro_request_raw(0, &RAW_dataX, &RAW_dataY, &RAW_dataZ);
        printf("Raw data: %f %f %f\n", RAW_dataX, RAW_dataY, RAW_dataZ);
        if(abs(RAW_dataX - data_avg_x)  <= 10){
            data_sum_x += RAW_dataX;
            counter_x++;
        }
        if(abs(RAW_dataY - data_avg_y) <= 10){
            data_sum_y += RAW_dataY;
            counter_y++;
        }
        if(abs(RAW_dataZ - data_avg_z) <= 10){
            data_sum_z += RAW_dataZ;
            counter_z++;
        }
        RAW_dataX_p = RAW_dataX;
        RAW_dataY_p = RAW_dataY;
        RAW_dataZ_p = RAW_dataZ;

        data_avg_x = data_sum_x/counter_x;
        data_avg_y = data_sum_y/counter_y;
        data_avg_z = data_sum_z/counter_z;
    }

    printf("Calibrated: %f %f %f\n", data_avg_x, data_avg_y, data_avg_z);
}

 //void control(){
//*/
//}
