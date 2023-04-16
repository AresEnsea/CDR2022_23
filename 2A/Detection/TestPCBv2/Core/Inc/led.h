/*
 * led.h
 *
 *  Created on: 24 janv. 2023
 *      Author: Victor Lesne
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32g0xx_hal.h"

#endif /* INC_LED_H_ */


/*
 * Dictionnaire des broches des leds :
 * LED 1	:	C3
 * LED 2	:	C0
 * LED 3	:	C13
 * LED 4	:	C10
 * LED 5	:	B7
 * LED 6	:	B4
 * LED 7	:	D3
 * LED 8	:	D0
 * LED 9	:	A15
 * LED 10	:	D8
 * LED 11	:	A9
 * LED 12	:	B14
 * LED 13	:	B11
 * LED 14	:	B1
 * LED 15	:	C4
 * LED 16	:	A5
 */

/**
 * @brief Initializes the LEDs at their ON state
 */
void LEDs_init(void);

/**
 * @brief This function toggle the LED corresponding to a given number
 * @param n the LED number between 1 and 16
 * @return 0:success, 1:out of range number
 */
uint8_t LEDtoggle(uint16_t n);
