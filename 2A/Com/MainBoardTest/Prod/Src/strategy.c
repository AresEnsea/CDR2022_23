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

    strategy->path[0] = bezier_new(206.931, 688.119, 306.931, 688.119, 649.240, 784.548, 751.081, 1039.701, 30);
    strategy->path[1] = bezier_new(751.081, 1039.701, 852.922, 1294.854, 993.040, 1436.579, 1179.333, 1505.294, 30);
    strategy->path[2] = bezier_new(1179.333, 1505.294, 1365.625, 1574.008, 1689.346, 1635.088, 1939.772, 1425.891, 30);
    strategy->path[3] = bezier_new(1939.772, 1425.891, 2190.198, 1216.693, 2168.820, 925.039, 2037.499, 741.800, 30);
    strategy->path[4] = bezier_new(2037.499, 741.800, 1906.179, 558.562, 1710.724, 439.457, 1002.202, 628.803, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;
    strategy->directions[3] = FORWARD;
    strategy->directions[4] = FORWARD;

    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;
    strategy->speeds[3] = 1000;
    strategy->speeds[4] = 1000;



    return strategy;
}
