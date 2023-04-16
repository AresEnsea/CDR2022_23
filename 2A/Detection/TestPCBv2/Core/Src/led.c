/*
 * led.c
 *
 *  Created on: 24 janv. 2023
 *      Author: Victor Lesne
 */

#include "led.h"

void LEDs_init(void){
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);		// LED 1
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);		// LED 2
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	// LED 3
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);	// LED 4

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);		// LED 5
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);		// LED 6

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);		// LED 7
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);		// LED 8

	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);	// LED 9

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);		// LED 10

	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);		// LED 11

	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	// LED 12
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);	// LED 13
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);		// LED 14

	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);		// LED 15
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);		// LED 16
}


uint8_t LEDtoggle(uint16_t n){
	if(n<=0 || n>16)
		return 1;

	switch (n){
	case 1:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
		break;
	case 2:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
		break;
	case 3:
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		break;
	case 4:
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_10);
			break;
	case 5:
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
			break;
	case 6:
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
			break;
	case 7:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_3);
			break;
	case 8:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
			break;
	case 9:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
			break;
	case 10:
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_8);
			break;
	case 11:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			break;
	case 12:
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
			break;
	case 13:
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
			break;
	case 14:
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
			break;
	case 15:
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
			break;
	case 16:
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			break;
	}

	return 0;
}
