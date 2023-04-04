#include "propulsion.h"


Robot robot;


void propulsion_initialize() {
    robot.leftMotor = (Stepper*) malloc(sizeof(Stepper));
    robot.rightMotor = (Stepper*) malloc(sizeof(Stepper));

    drv8825_initialize(
        robot.leftMotor,
        L_MOTOR_EN_Pin, L_MOTOR_EN_GPIO_Port,
        L_MOTOR_DIR_Pin, L_MOTOR_DIR_GPIO_Port,
        &L_MOTOR_TIMER, TIM_CHANNEL_1
    );
    drv8825_initialize(
        robot.rightMotor,
        R_MOTOR_EN_Pin, R_MOTOR_EN_GPIO_Port,
        R_MOTOR_DIR_Pin, R_MOTOR_DIR_GPIO_Port,
        &R_MOTOR_TIMER, TIM_CHANNEL_4
    );
}


void propulsion_enableMotors() {
    drv8825_enable(robot.leftMotor);
    drv8825_enable(robot.rightMotor);
}


void propulsion_disableMotors() {
    drv8825_disable(robot.leftMotor);
    drv8825_disable(robot.rightMotor);
}


void propulsion_setSpeeds(float left, float right) {
    float greatestAbsoluteSpeed = fabs(fabs(left)>fabs(right)?left:right);
    if (greatestAbsoluteSpeed > MAX_MOTOR_SPEED) {
        left = left/greatestAbsoluteSpeed*MAX_MOTOR_SPEED;
        right = right/greatestAbsoluteSpeed*MAX_MOTOR_SPEED;
    }

    robot.leftSpeed = left;
    drv8825_setDirection(robot.leftMotor, (left < 0)?NEGATIVE:POSITIVE);
    drv8825_setRotationSpeed(robot.leftMotor, 60 * fabsf(left) / (2*M_PI*WHEEL_RADIUS));

    robot.rightSpeed = right;
    drv8825_setDirection(robot.rightMotor, (right < 0)?NEGATIVE:POSITIVE);
    drv8825_setRotationSpeed(robot.rightMotor, 60 * fabsf(right) / (2*M_PI*WHEEL_RADIUS));
}


void propulsion_updatePosition(float dt) {
    float left = robot.leftSpeed * dt;
    float right = robot.rightSpeed * dt;

    float r =  (left + right)/2;
    float alpha = (right - left)/ENTRAXE_MOTOR;

    robot.position.x += r * cos(robot.angle + alpha/2);
    robot.position.y += r * sin(robot.angle + alpha/2);
    robot.angle += alpha;

    if (robot.angle > M_PI)
        robot.angle = robot.angle - M_TWOPI;
    else if (robot.angle < -M_PI)
        robot.angle = robot.angle + M_TWOPI;
}


float getAngleError(Bezier* b, float t, Vector2 p, Direction dir) {
    Vector2 tangent = bezier_deriv1(b, t);
    Vector2 displacement = vector2_diff(p, robot.position);

    float sign = vector2_cross(displacement, tangent); // (vaut -1, 0 ou 1)
    sign /= (sign != 0)?fabs(sign):1;
    float signedDistance = sign * vector2_norm(displacement);

    DEBUG_PROPULSION("dist: %.1fmm, ", signedDistance);

    float angleError = (vector2_angle(tangent) - atan(signedDistance / 200.0))
            - (robot.angle + (dir==BACKWARD?M_PI:0));
    angleError = standardAngle(angleError); // (entre -pi et pi)

    return angleError;
}


float getRobotSpeed(float t, float angleError, Direction dir, float initialSpeed, float finalSpeed) {
    float slowDownFactor = 1 - fabs(angleError/M_PI) * 10;
    slowDownFactor = slowDownFactor>0?slowDownFactor:0;

    float speed = (dir==BACKWARD?-1:1) * slowDownFactor;
    if (initialSpeed < 1 && finalSpeed < 1)
        speed *= 600*(1-t) + 20;
    else
        speed *= initialSpeed*(1-t) + finalSpeed*t + 50;

    // Limite les accelerations brutales
    if (fabs(speed) > robot.measuredSpeed + 15)
        speed = (robot.measuredSpeed + 15) * fabs(speed) / speed;

    return speed;
}


void calculateMotorSpeeds(float* leftSpeed, float* rightSpeed, Bezier* b, float t, Direction dir, float speed) {
    float k = bezier_curvature(b, t);

    *leftSpeed = speed * (1 + dir*ENTRAXE_MOTOR*k/2);
    *rightSpeed = speed * (1 - dir*ENTRAXE_MOTOR*k/2);
}


void addCorrection(float* leftSpeed, float* rightSpeed, float angleError) {
    float correction = -angleError*200; // contre réaction

    if (fabs(correction) > MAX_CORRECTION) {
        correction *= MAX_CORRECTION / abs(correction);
    }

    DEBUG_PROPULSION("corr: %.1fmm\r\n", correction);

    *leftSpeed -= correction;
    *rightSpeed += correction;
}


// C'est ici que la magie a lieu !
float propulsion_followBezier(Bezier* b, Direction dir, float initialSpeed, float finalSpeed, bool reverse) {
    float t = bezier_project(b, robot.position, 0.0001); // (entre 0 et 1)
    Vector2 p = bezier_eval(b, t); // Point de la courbe le plus proche du robot

    DEBUG_PROPULSION("t: %.5f, x: %.1fmm, y: %.1fmm, a: %.1fdeg, ", t, robot.position.x, robot.position.y, robot.angle / M_PI / 2 * 360);

    // Erreur d'orientation
    float angleError = getAngleError(b, t, p, dir);

    if (reverse) {
    	angleError = 0;
    }

    // Vitesse globale du robot
    float speed = getRobotSpeed(t, angleError, dir, initialSpeed, finalSpeed);

    if (reverse) {
    	speed *= -1;
    }

    float leftSpeed, rightSpeed;

    // Boucle ouverte
    calculateMotorSpeeds(&leftSpeed, &rightSpeed, b, t, dir, speed);

    // Boucle fermée
    addCorrection(&leftSpeed, &rightSpeed, angleError);

    // Commande des moteurs
    propulsion_setSpeeds(leftSpeed, rightSpeed);

    return t;
}


float standardAngle(float angle) {
    return fmod(fmod(angle + M_PI, M_TWOPI) + M_TWOPI, M_TWOPI) - M_PI;
}
