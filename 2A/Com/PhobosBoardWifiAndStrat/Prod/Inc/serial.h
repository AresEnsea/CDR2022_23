#ifndef _SERIAL_
#define _SERIAL_

#include "usart.h"
#include "gpio.h"
#include "robot.h"
#include "lidar.h"
#include <stdio.h>

extern uint8_t lidarData;
extern uint8_t USARTprintChange;
extern uint8_t wifiDataRX;
extern uint8_t wifiDataTX;
extern uint8_t armData;
extern uint8_t pData[1];

void parser32(uint32_t *ptr);
int serial_send(uint8_t *ptr, int len, int uartPort);
void WifiUartTXActivation(uint8_t activate);

#endif
