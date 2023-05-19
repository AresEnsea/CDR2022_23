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

    //strategy->path[0] = bezier_new(93.424, 18.440, 97.918, 364.078, 637.740, 290.185, 968.473, 273.860, 30);
    //strategy->path[1] = bezier_new(968.473, 273.860, 1299.207, 257.535, 1701.727, 151.404, 1797.497, 80.915, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;

    strategy->speeds[0] = 192;
    strategy->speeds[1] = 192;



    return strategy;
	}
	else if(num == 2 || num == 7)
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

    //strategy->path[0] = bezier_new(1910.186, 16.835, 1913.751, 500.096, 1467.877, 473.925, 1263.293, 472.532, 30);
    //strategy->path[1] = bezier_new(1263.293, 472.532, 1058.708, 471.139, 659.790, 506.976, 595.499, 356.912, 30);
    //strategy->path[2] = bezier_new(595.499, 356.912, 531.208, 206.847, 163.527, 89.308, 163.527, 89.308, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 388;
    strategy->speeds[1] = 388;
    strategy->speeds[2] = 350;



    return strategy;
	}
	else if(num == 3 || num == 6)
	{	Strategy* strategy = (Strategy*) malloc(sizeof(Strategy));
    strategy->length = 2;
    strategy->path = (Bezier**) malloc(sizeof(Bezier*) * strategy->length);

    strategy->directions = (Direction*) malloc(sizeof(Direction) * strategy->length);
    strategy->speeds = (float*) calloc(strategy->length+1, sizeof(float));

    strategy->onSiteActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onSiteActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));
    strategy->onMoveActions = (uint8_t**) malloc(sizeof(uint8_t*) * (strategy->length+1));
    strategy->onMoveActionsLengths = (int*) calloc((strategy->length+1), sizeof(int));

    //strategy->path[0] = bezier_new(1095.392, 1841.935, 1098.577, 1678.371, 1099.549, 1671.753, 1105.089, 1399.884, 30);
    //strategy->path[1] = bezier_new(1105.089, 1399.884, 1110.630, 1128.015, 1407.093, 1071.997, 1407.093, 1071.997, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = BACKWARD;

    strategy->speeds[0] = 496;
    strategy->speeds[1] = 496;



    return strategy;
	}
	else if (num == 4 || num == 5)
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

    strategy->path[0] = bezier_new(1913.809, 18.314, 1913.980, 60.228, 1912.586, 48.561, 1911.417, 99.091, 30);
    strategy->path[1] = bezier_new(1911.417, 99.091, 1910.248, 149.621, 1897.062, 173.943, 1908.884, 205.469, 30);
    strategy->path[2] = bezier_new(1908.884, 205.469, 1920.707, 236.995, 2015.285, 276.402, 2015.285, 276.402, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 199;
    strategy->speeds[1] = 170;
    strategy->speeds[2] = 167;

    return strategy;
	}
}


