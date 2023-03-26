#include "serial.h"

uint8_t lidarData;
uint8_t armData;
uint8_t USARTprintChange;
uint8_t wifiDataRX;
uint8_t wifiDataTX;
uint8_t pData[1];

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
	else if (uartPort == 6) {
		uint8_t action = *ptr;
		if(action == 0x69)
		{
			HAL_UART_Transmit(&huart6, "1005", 4, 1);
		}
		else if(action == 0x70)
		{
			HAL_UART_Transmit(&huart6, "1010", 4, 1);
		}
		/*if (robot.team == BLUE) {
			if (action == 0xA1) {
				action = 0xA2;
			} else if (*ptr == 0xCA) {
				action = 0xCB;
			} else if (*ptr == 0xCB) {
				action = 0xCA;
			}
		}*/
		//HAL_UART_Transmit(&huart6, &action, len, HAL_MAX_DELAY);
		//printf("Sending %x\r\n", *ptr);
	}
	return len;
}

void WifiUartActivation(uint8_t activate){//active 0 send 1 byte, 1 send 100
	if(activate==1){
		wifiDataTX=0x7F;
	}else{wifiDataTX=0x7E;}
	serial_send(&wifiDataTX, 1, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	if(huart->Instance == UART4){
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		int id = lidarData >> 4;
		uint8_t dist = (lidarData & 0xF) << 2;
		lidar_updateDistance(id, dist);
		HAL_UART_Receive_IT(&huart4, &lidarData, 1);
	}

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
			wifiDataTX=0x10;
			serial_send(&wifiDataTX,1,1);
		}
		else if(wifiDataRX==0xE0 && byteCount<1){
			byteCount=1;
			//wifiDataTX=224+byteCount;
			//serial_send(&wifiDataTX,1,1);
		}
		else if(byteCount>0){
			rec += (wifiDataRX & 0x7F) << (byteCount-1)*7;
			//wifiDataTX=224+byteCount+1;
			//serial_send(&wifiDataTX,1,1);
			byteCount++;
			if(byteCount>5){
				byteCount=0;

				char buf[20];
				uint16_t size = snprintf(buf,20,"%3.3lu\r\n",rec);
				HAL_UART_Transmit(&huart2, buf, size, HAL_MAX_DELAY);
				rec=0;
			}
		}
		HAL_UART_Receive_IT(&huart1, &wifiDataRX, 1);
	}
}
