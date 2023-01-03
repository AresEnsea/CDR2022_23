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

    strategy->onSiteActionsLengths[0] = 1;
    strategy->onSiteActions[0] = (uint8_t*) malloc(sizeof(uint8_t) * 4);
    strategy->onSiteActions[0][0] = 0x31;
    strategy->onSiteActions[0][1] = 0x30;
    strategy->onSiteActions[0][2] = 0x32;
    strategy->onSiteActions[0][3] = 0x35;

    strategy->speeds[1] = 204;
    strategy->speeds[2] = 204;
    strategy->speeds[3] = 204;

    return strategy;
}

/*#include "strategy.h"

Strategy* strategy_initialize() {
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 2;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(24.917, 922.769, 715.421, 718.021, 864.985, 2163.205, 1232.938, 1397.626, 30);
    strategy->path[1] = bezier_new(1232.938, 1397.626, 1600.890, 632.047, 1710.682, 1456.973, 1710.682, 1456.973, 30);

    strategy->directions[0] = BACKWARD;
    strategy->directions[1] = BACKWARD;


    strategy->onSiteActionsLengths[1] = 1;
    strategy->onSiteActions[1] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[1][0] = 0x78;
    strategy->onSiteActionsLengths[2] = 1;
    strategy->onSiteActions[2] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[2][0] = 0x52;


    return strategy;
}

#include "strategy.h"

Strategy* strategy_initialize() {
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 1;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(431.449, 901.997, 1121.953, 697.249, 1161.721, 1949.555, 1529.674, 1183.976, 30);

    strategy->directions[0] = BACKWARD;


    strategy->onSiteActionsLengths[0] = 1;
    strategy->onSiteActions[0] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[0][0] = 0x74;
    strategy->onSiteActionsLengths[1] = 1;
    strategy->onSiteActions[1] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[1][0] = 0x78;


    return strategy;
}*/
