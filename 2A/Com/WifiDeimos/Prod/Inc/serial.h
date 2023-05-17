#ifndef _SERIAL_
#define _SERIAL_

#include "usart.h"
#include "gpio.h"
//#include "robot.h"
//#include "lidar.h"
#include <stdio.h>

#define ULTRA_BUFFER_SIZE 38

extern uint8_t lidarData;
extern uint8_t wifiDataRX;
extern uint8_t wifiDataTX;
extern uint8_t ultraBuffer[ULTRA_BUFFER_SIZE];
extern uint8_t armData;
extern uint8_t pData[0];
extern int printMode;

int serial_send(uint8_t *ptr, int len, int uartPort);
void parser32(uint32_t *ptr);

#endif
