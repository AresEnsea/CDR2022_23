#include "strategy.h"

Strategy* strategy_initialize(int num) {
	if(num == 0 || num == 9)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 3;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(1082.584, 1835.955, 1081.461, 1658.989, 1081.549, 1857.035, 1078.740, 1420.466, 30);
    strategy->path[1] = bezier_new(1078.740, 1420.466, 1075.931, 983.896, 2221.088, 1360.702, 2418.386, 1412.953, 30);
    strategy->path[2] = bezier_new(2418.386, 1412.953, 2615.684, 1465.203, 2620.330, 1570.430, 2706.038, 1698.907, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 514;
    strategy->speeds[1] = 514;
    strategy->speeds[2] = 514;



    return strategy;
	}
	else if(num == 2 || num == 7)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 2;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(1082.584, 1835.955, 1081.461, 1658.989, 1081.549, 1857.035, 1078.740, 1420.466, 30);
    strategy->path[1] = bezier_new(1078.740, 1420.466, 1075.931, 983.896, 2811.318, 1352.938, 2804.360, 1789.922, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;

    strategy->speeds[0] = 400;
    strategy->speeds[1] = 400;



    return strategy;
	}
	else if(num == 3 || num == 6)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 2;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(1071.910, 1824.719, 1065.169, 1689.888, 1064.045, 1712.360, 1054.494, 1597.191, 30);
    strategy->path[1] = bezier_new(1054.494, 1597.191, 1044.944, 1482.022, 1051.264, 1537.921, 1042.837, 1467.697, 30);

    strategy->directions[0] = BACKWARD;
    strategy->directions[1] = BACKWARD;

    strategy->speeds[0] = 536;
    strategy->speeds[1] = 536;



    return strategy;
	}
	else if (num == 4 || num == 5)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 1;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(2891.389, 602.294, 2513.822, 611.475, 2332.630, 1227.539, 1921.372, 1243.315, 30);

    strategy->directions[0] = FORWARD;

    strategy->speeds[0] = 9;

    return strategy;
	}
}


