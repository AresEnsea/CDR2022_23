#ifndef _SERIAL_
#define _SERIAL_

#include "usart.h"
#include "gpio.h"
#include "robot.h"
#include "lidar.h"
#include "strategy.h"
#include "symetry.h"
#include <stdio.h>

extern uint8_t lidarData;
extern uint8_t wifiData[3];
extern uint8_t armData;
/*extern uint8_t pData[1];
extern Strategy* strategy;*/

int serial_send(uint8_t *ptr, int len, int uartPort);

#endif
