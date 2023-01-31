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

    strategy->path[0] = bezier_new(1063.496, 1830.780, 1056.849, 1715.332, 1101.176, 1450.091, 758.168, 1471.801, 30);

    strategy->directions[0] = BACKWARD;




    return strategy;
}
/*#include "strategy.h"

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

    strategy->path[0] = bezier_new(1063.496, 1830.780, 629.549, 1836.994, 374.173, 1414.483, 553.420, 1041.534, 30);

    strategy->directions[0] = BACKWARD;


    strategy->onSiteActionsLengths[1] = 1;
    strategy->onSiteActions[1] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[1][0] = 0x68;


    return strategy;
}*/
