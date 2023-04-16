#include "serial.h"
#include "strategy.h"
#include <string.h>

uint8_t lidarData;
uint8_t armData;
uint8_t wifiDataRX;
uint8_t wifiDataTX;
static uint8_t wifiDataTXMany[8];
static uint8_t lastStrategyCode = 0;
static float strategyBezierData[8];
static Bezier* nextPointBezier = NULL;
static float nextPointSpeed = 0.0f;
static int currentStrategyPoint = 0;

void parser32(uint32_t *ptr){
	uint8_t trigger=0xE0;
	uint8_t byte7No0= (*ptr & 0x0000007F) + 0x0000080;
	uint8_t byte7No1= ((*ptr>>7) & 0x0000007F) + 0x0000080;
	uint8_t byte7No2= ((*ptr>>14) & 0x0000007F) + 0x0000080;
	uint8_t byte7No3= ((*ptr>>21) & 0x0000007F) + 0x0000080;
	uint8_t byte7No4= ((*ptr>>28) & 0x0000007F) + 0x0000080;
	serial_send(&trigger,1,1);
	/*while(wifiDataRX!=0xE1){

		}*/
	serial_send(&byte7No0,1,1);
	/*while(wifiDataRX!=0xE2){

		}*/
	serial_send(&byte7No1,1,1);
	/*while(wifiDataRX!=0xE3){

		}*/
	serial_send(&byte7No2,1,1);
	/*while(wifiDataRX!=0xE4){

		}*/
	serial_send(&byte7No3,1,1);
	/*while(wifiDataRX!=0xE5){

		}*/
	serial_send(&byte7No4,1,1);
}

int serial_send(uint8_t *ptr, int len, int uartPort) {
	if (uartPort == 4)
		HAL_UART_Transmit(&huart4, ptr, len, HAL_MAX_DELAY);
	else if (uartPort == 5)
		HAL_UART_Transmit(&huart5, ptr, len, HAL_MAX_DELAY);
	else if (uartPort == 1){
		HAL_UART_Transmit(&huart1, ptr, len, HAL_MAX_DELAY);
	}
	/*else if (uartPort == 6) {
		uint8_t action = *ptr;
		if(action == 0x69)
		{
			HAL_UART_Transmit(&huart6, "1005", 4, 1);
		}
		else if(action == 0x70)
		{
			HAL_UART_Transmit(&huart6, "1010", 4, 1);
		}
		if (robot.team == BLUE) {
			if (action == 0xA1) {
				action = 0xA2;
			} else if (*ptr == 0xCA) {
				action = 0xCB;
			} else if (*ptr == 0xCB) {
				action = 0xCA;
			}
		}
		//HAL_UART_Transmit(&huart6, &action, len, HAL_MAX_DELAY);
		//printf("Sending %x\r\n", *ptr);
	}*/
	return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	/*if(huart->Instance == UART4){
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		int id = lidarData >> 4;
		uint8_t dist = (lidarData & 0xF) << 2;
		lidar_updateDistance(id, dist);
		HAL_UART_Receive_IT(&huart4, &lidarData, 1);
	}*/

	/*if(huart->Instance == USART6){
		if (armData == 0xFF) {
			HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
			robot.waitingForOnSiteAction = false;
			robot.waitingForOnMoveAction = false;
			//printf("Arm: Continue.\r\n");
		}
		HAL_UART_Receive_IT(&huart6, &armData, 1);
	}*/

	if(huart->Instance == USART1){
		static int byteCount=0;
		static uint32_t rec=0;
		if(wifiDataRX==0xC0 && byteCount<1){
			wifiDataTX=0x40;
			wifiDataTXMany[0] = 0;
			wifiDataTXMany[1] = 0;
			wifiDataTXMany[2] = 0;
			wifiDataTXMany[3] = 0;
			wifiDataTXMany[4] = 3;
			wifiDataTXMany[5] = 'u';
			wifiDataTXMany[6] = 'w';
			wifiDataTXMany[7] = 'u';

			serial_send(wifiDataTXMany,sizeof(wifiDataTXMany),1);
		}
		else if(wifiDataRX==0xE0 && byteCount<1){
			byteCount=1;
			//wifiDataTX=224+byteCount;
			//serial_send(&wifiDataTX,1,1);
		}
		else if(wifiDataRX==0xD0 && byteCount <1) {
			byteCount = 1;
		}
		else if(wifiDataRX >= 0xD1 && wifiDataRX <= (0xD1 + 8 + 1) && byteCount < 1) {
			lastStrategyCode = wifiDataRX;
			byteCount = 5;
		}
		else if(byteCount>0) {
			byteCount--;
			rec += (wifiDataRX & 0x7F) << (byteCount * 7);
			//wifiDataTX=224+byteCount+1;
			//serial_send(&wifiDataTX,1,1);
			if(byteCount == 0)
			{
				switch(lastStrategyCode)
				{
				case 0xD1:
					if(strategy_not_in_use == NULL && use_new_strategy == 0 && moving_new_strategy == 0)
					{
						writing_new_strategy = 1;
						strategy_not_in_use = strategy_new(rec);
						currentStrategyPoint = 0;
					}
					else
					{
						writing_new_strategy = 0;
					}
					break;
				case 0xD2:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[0], &rec, 4);
					}
					break;
				case 0xD3:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[1], &rec, 4);
					}
					break;
				case 0xD4:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[2], &rec, 4);
					}
					break;
				case 0xD5:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[3], &rec, 4);
					}
					break;
				case 0xD6:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[4], &rec, 4);
					}
					break;
				case 0xD7:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[5], &rec, 4);
					}
					break;
				case 0xD8:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[6], &rec, 4);
					}
					break;
				case 0xD9:
					if(writing_new_strategy)
					{
						memcpy(&strategyBezierData[7], &rec, 4);
						nextPointBezier = bezier_new(
								strategyBezierData[0], strategyBezierData[1],
								strategyBezierData[2], strategyBezierData[3],
								strategyBezierData[4], strategyBezierData[5],
								strategyBezierData[6], strategyBezierData[7], 30);
					}
					break;
				case 0xDA:
					if(writing_new_strategy)
					{
						memcpy(&nextPointSpeed, &rec, 4);
						strategy_set_point_dirspeed(strategy_not_in_use, currentStrategyPoint, nextPointBezier, nextPointSpeed);
						{

							wifiDataTXMany[0] = 0;
							wifiDataTXMany[1] = 0;
							wifiDataTXMany[2] = 0;
							wifiDataTXMany[3] = 0;
							wifiDataTXMany[4] = 3;
							wifiDataTXMany[5] = '=';
							wifiDataTXMany[6] = '0' + (currentStrategyPoint / 10);
							wifiDataTXMany[7] = '0' + (currentStrategyPoint % 10);

							serial_send(wifiDataTXMany,sizeof(wifiDataTXMany),1);
						}
						currentStrategyPoint += 1;

						if(currentStrategyPoint == strategy_not_in_use->length)
						{
							use_new_strategy = 1;
							writing_new_strategy = 0;

							wifiDataTXMany[0] = 0;
							wifiDataTXMany[1] = 0;
							wifiDataTXMany[2] = 0;
							wifiDataTXMany[3] = 0;
							wifiDataTXMany[4] = 3;
							wifiDataTXMany[5] = 'O';
							wifiDataTXMany[6] = 'K';
							wifiDataTXMany[7] = '!';

							serial_send(wifiDataTXMany,sizeof(wifiDataTXMany),1);
						}
					}
				default:
					break;
				}
				lastStrategyCode = 0;

				//char buf[20];
				//uint16_t size = snprintf(buf,20,"%3.3lu\r\n",rec);
				//HAL_UART_Transmit(&huart2, buf, size, HAL_MAX_DELAY);
				rec=0;
			}
		}
		HAL_UART_Receive_IT(&huart1, &wifiDataRX, 1);
	}
}
