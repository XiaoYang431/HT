#ifndef __BLUETEECH_H__
#define __BLUETEECH_H__

#include "ht32.h"

typedef struct {
    double latitude;
    double longitude;
    uint8_t valid;
} GPS_Coordinate;

void BLUETEETH_Init(void);
void GPS_To_BlueTooth(void);
void blueteech_delay(vu32 count);
uint8_t BM7701_00_1_bleProcess(void);
void Get_blueteech_status(void);
void processBluetoothGPS(void);
GPS_Coordinate parseGPSMessage(const char* msg);
#endif
