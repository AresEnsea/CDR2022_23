#include "strategy.h"

Strategy* strategy_initialize(int num) {
	if(num == 0 || num == 9)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));//x=93 y=22, fin x=1785 y=81
    strategy->length = 2;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(93.424, 18.440, 97.918, 364.078, 637.740, 290.185, 968.473, 273.860, 30);
    strategy->path[1] = bezier_new(968.473, 273.860, 1299.207, 257.535, 1701.727, 151.404, 1797.497, 80.915, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;

    strategy->speeds[0] = 192;
    strategy->speeds[1] = 192;



    return strategy;
	}
	else if(num == 2 || num == 7)
	{
    Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 4;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    strategy->path[0] = bezier_new(1906.675, 9.815, 1908.757, 383.939, 1420.051, -70.795, 943.692, -21.386, 30);
    strategy->path[1] = bezier_new(943.692, -21.386, 467.333, 28.023, 160.805, 74.565, 274.415, 247.979, 30);
    strategy->path[2] = bezier_new(274.415, 247.979, 388.026, 421.393, 1927.433, 472.292, 2117.736, 475.220, 30);
    strategy->path[3] = bezier_new(2117.736, 475.220, 2308.039, 478.147, 2764.766, 472.292, 2764.766, 472.292, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;
    strategy->directions[3] = FORWARD;

    strategy->speeds[0] = 192;
    strategy->speeds[1] = 192;
    strategy->speeds[2] = 192;
    strategy->speeds[3] = 205;



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


