#include "strategy.h"

Strategy* strategy_initialize() {
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 3;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(203.960, 800.990, 384.158, 913.861, 216.832, 1074.257, 290.099, 1163.366, 30);
    strategy->path[1] = bezier_new(290.099, 1163.366, 363.366, 1252.475, 640.099, 1317.822, 748.515, 1197.525, 30);
    strategy->path[2] = bezier_new(748.515, 1197.525, 856.931, 1077.228, 944.554, 965.842, 677.228, 829.208, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[1] = 204;
    strategy->speeds[2] = 204;
    strategy->speeds[3] = 204;



    return strategy;
}
