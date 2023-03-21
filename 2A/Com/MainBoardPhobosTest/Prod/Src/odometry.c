#include "odometry.h"


int16_t getRightEncoderCount() {
	int16_t count = (int16_t) R_ENCODER.Instance->CNT;
	R_ENCODER.Instance->CNT = 0;
	return count;
}


int16_t getLeftEncoderCount() {
	int16_t count = -1 * (int16_t) L_ENCODER.Instance->CNT;
	L_ENCODER.Instance->CNT = 0;
	return count;
}

void odometry_updatePosition() {
	float coeff = M_TWOPI * ODOMETRY_RADIUS / TICKS_PER_REV;
	float leftMov = getLeftEncoderCount() * coeff;
	float rightMov = getRightEncoderCount() * coeff;

	float r =  (leftMov + rightMov)/2;
	float alpha = (leftMov - rightMov)/ENTRAXE_ODOMETRY;

	float deltaX = r * cos(robot.angle + alpha/2);
	float deltaY = r * sin(robot.angle + alpha/2);

	float dt = 0.01; // 10 ms

	robot.position.x += deltaX;
	robot.position.y += deltaY;
	robot.angle += alpha;

	float currentSpeed = vector2_norm(vector2_new(deltaX, deltaY)) / dt;
	robot.measuredSpeed = robot.measuredSpeed*0.8 + currentSpeed*0.2;

	if (robot.angle > M_PI)
		robot.angle = robot.angle - M_TWOPI;
	else if (robot.angle < -M_PI)
		robot.angle = robot.angle + M_TWOPI;

	DEBUG_ODOMETRY("x: %.1fmm, y: %.1fmm, angle: %.1fdeg\r\n", robot.position.x, robot.position.y, robot.angle/2/M_PI*360);
}


void odometry_setPosition(float x, float y) {
	robot.position.x = x;
	robot.position.y = y;
}


void odometry_setAngle(float angle) {
	robot.angle = angle;
}
