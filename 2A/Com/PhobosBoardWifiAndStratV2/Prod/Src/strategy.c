#include "strategy.h"

Strategy* strategy_initialize(int num) {
	if(num == 0 || num == 9)
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

    strategy->path[0] = bezier_new(1086.139, 1840.594, 1097.030, 1035.644, 1941.089, 1430.693, 1956.436, 1095.050, 30);
    strategy->path[1] = bezier_new(1956.436, 1095.050, 1971.782, 759.406, 880.693, 685.149, 897.030, 1051.980, 30);
    strategy->path[2] = bezier_new(897.030, 1051.980, 913.366, 1418.812, 2153.465, 555.941, 2117.822, 1054.950, 30);
    strategy->path[3] = bezier_new(2117.822, 1054.950, 2082.178, 1553.960, 748.515, 1063.861, 1087.129, 1711.386, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;
    strategy->directions[3] = FORWARD;

    strategy->speeds[0] = 1000;
    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;
    strategy->speeds[3] = 1000;



    return strategy;
	}
	else if(num == 2 || num == 7)
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

		strategy->path[0] = bezier_new(1081.176, 1847.059, 588.235, 1409.412, 1644.706, 1310.588, 1187.059, 1843.529, 30);

		strategy->directions[0] = FORWARD;

		strategy->speeds[0] = 105;



		return strategy;
	}
	else if(num == 3 || num == 6)
	{

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


