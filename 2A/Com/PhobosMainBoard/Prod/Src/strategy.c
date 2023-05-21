#include "strategy.h"

Strategy* strategy_initialize() {
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 5;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(1908.024, 255.474, 1908.024, 255.474, 1941.408, 605.693, 1444.101, 486.766, 30);
    strategy->path[1] = bezier_new(1444.101, 486.766, 946.795, 367.838, 690.577, 968.001, 619.452, 480.539, 30);
    strategy->path[2] = bezier_new(619.452, 480.539, 548.327, -6.922, 264.346, 4.676, 264.346, 4.676, 30);
    strategy->path[3] = bezier_new(264.346, 4.676, 264.346, 4.676, 995.275, 319.472, 995.275, 319.472, 30);
    strategy->path[4] = bezier_new(995.275, 319.472, 995.275, 319.472, 1907.731, 602.071, 1907.232, 254.630, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;
    strategy->directions[3] = BACKWARD;
    strategy->directions[4] = BACKWARD;

    strategy->speeds[0] = 1000;
    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;
    strategy->speeds[3] = 1000;
    strategy->speeds[4] = 1000;



    return strategy;
}
