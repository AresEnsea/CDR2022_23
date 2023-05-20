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

    strategy->path[0] = bezier_new(1908.688, 253.753, 1865.272, 544.973, 1779.838, 690.077, 1338.819, 702.890, 30);
    strategy->path[1] = bezier_new(1338.819, 702.890, 897.799, 715.704, 697.448, 335.383, 542.376, 190.856, 30);
    strategy->path[2] = bezier_new(542.376, 190.856, 387.305, 46.328, 318.076, 24.629, 166.511, 3.935, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 1000;
    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;



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

    strategy->path[0] = bezier_new(2878.517, 722.484, 2529.149, 715.686, 1856.484, 831.876, 1704.227, 835.326, 30);
    strategy->path[1] = bezier_new(1704.227, 835.326, 1551.970, 838.776, 1190.315, 859.573, 784.791, 799.324, 30);
    strategy->path[2] = bezier_new(784.791, 799.324, 379.267, 739.076, 777.457, 113.298, 208.660, 119.040, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 1000;
    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;



    return strategy;
	}
}


