#pragma once
#include <PZEM004Tv30.h>

typedef struct __attribute__((packed))
{
    uint16_t chipId;
    uint8_t sensor_type;
    uint16_t volt; 
    float ampe;
    uint16_t wat; 
    uint8_t frequency;
    float kwh;
    byte device_status;
    uint16_t threshold;
} metterData;

extern metterData sensorData;
extern float cospi_slave;

void pzemInit();
void pzemReadAndUpdate();
void pzemDisplayHome();
void pzemResetEnergy();
