#include "serial.h"
uint8_t lidarData;
uint8_t armData;

int serial_send(uint8_t *ptr, int len, int uartPort) {
	if (uartPort == 4)
		HAL_UART_Transmit(&huart4, ptr, len, HAL_MAX_DELAY);
	else if (uartPort == 5)
		HAL_UART_Transmit(&huart5, ptr, len, HAL_MAX_DELAY);
	else if (uartPort == 6) {
		uint8_t action = *ptr;
		if (robot.team == PURPLE) {
			if (action == 0xA1) {
				action = 0xA2;
			} else if (*ptr == 0xCA) {
				action = 0xCB;
			} else if (*ptr == 0xCB) {
				action = 0xCA;
			}
		}
		HAL_UART_Transmit(&huart6, &action, len, HAL_MAX_DELAY);
		//printf("Sending %x\r\n", *ptr);
	}
	return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
	if(huart->Instance == UART4){
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		int id = lidarData >> 4;
		uint8_t dist = (lidarData & 0xF) << 2;
		lidar_updateDistance(id, dist);
		HAL_UART_Receive_IT(&huart4, &lidarData, 1);
	}

	if(huart->Instance == USART6){
		if (armData == 0xFF) {
			HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
			robot.waitingForOnSiteAction = false;
			robot.waitingForOnMoveAction = false;
			//printf("Arm: Continue.\r\n");
		}

		HAL_UART_Receive_IT(&huart6, &armData, 1);
	}
}
