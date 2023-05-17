#include "avoidance.h"

AvoidanceState avoidanceState;
Bezier* avoidanceCurve;
float backtrackingAim;
float avoidanceSpeedFactor;

void avoidance_initialize() {
	avoidanceState = PATH_CLEAR;
	avoidanceCurve = bezier_newEmpty();
	backtrackingAim = 0;
	avoidanceSpeedFactor = 1;
}


void avoidance_update(float t, Direction dir) {
	int distance = lidar_getDistance(dir); uint8_t d2 = (uint8_t) distance;
	if(distance == 0)
		distance = 1000;
	uint8_t buffy[30];
	ITM_Port32(31) = distance;
	printf("distance %d\r\n",distance);
	//sprintf(buffy,"distance %d\r\n",distance);
	//HAL_UART_Transmit(&huart1, buffy,sizeof(buffy),HAL_MAX_DELAY);
	if ((avoidanceState == PATH_CLEAR || avoidanceState == PATH_CROWDED) && distance < LIDAR_THRESHOLD) {
		avoidanceState = PATH_OBSTRUCTED;
		printf("Obstacle detected. Waiting 2s...\r\n");
		propulsion_setSpeeds(0, 0);
		HAL_Delay(2000);
	}

	if ((avoidanceState == PATH_CLEAR || avoidanceState == PATH_OBSTRUCTED) && distance > LIDAR_THRESHOLD && distance < LIDAR_MAX_DISTANCE) {
		avoidanceSpeedFactor = (distance - LIDAR_THRESHOLD) / (LIDAR_MAX_DISTANCE - LIDAR_THRESHOLD);
		avoidanceState = PATH_CROWDED;
	}

	if (avoidanceState == PATH_CROWDED && distance > LIDAR_MAX_DISTANCE) {
		avoidanceState = PATH_CLEAR;
	}

	if (avoidanceState == PATH_OBSTRUCTED && distance >= LIDAR_THRESHOLD) {
		avoidanceState = PATH_CLEAR;
		printf("Path cleared.\r\n");
	}

	if (avoidanceState == PATH_OBSTRUCTED && distance < LIDAR_THRESHOLD) {
		avoidanceState = BACKTRACKING;
		printf("Backtracking...\r\n");
		backtrackingAim = t - 0.2;
		if (backtrackingAim < 0) {
			backtrackingAim = 0;
		}
	}

	if (avoidanceState == BACKTRACKING && t <= backtrackingAim + 0.01) {
		avoidanceState = PATH_CLEAR;
		printf("Trying again...\r\n");
	}
}
