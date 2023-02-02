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

    strategy->path[0] = bezier_new(87.235, 17.724, 89.489, 421.564, 228.773, 616.263, 648.375, 617.202, 30);
    strategy->path[1] = bezier_new(648.375, 617.202, 1067.978, 618.141, 1375.371, 655.786, 1375.371, 655.786, 30);
    strategy->path[2] = bezier_new(1375.371, 655.786, 1375.371, 655.786, 1864.985, 649.852, 1933.234, 145.401, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;

    strategy->speeds[0] = 9;
    strategy->speeds[1] = 110;
    strategy->speeds[2] = 110;
    strategy->speeds[3] = 85;

    strategy->onSiteActionsLengths[1] = 1;
    strategy->onSiteActions[1] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[1][0] = 0x69;
    strategy->onSiteActionsLengths[2] = 1;
    strategy->onSiteActions[2] = (uint8_t*) malloc(sizeof(uint8_t) * 1);
    strategy->onSiteActions[2][0] = 0x70;


    return strategy;
	}
	else if(num == 1 || num == 8)
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

    strategy->path[0] = bezier_new(1087.235, 38.495, 1089.489, 418.596, 1338.565, 975.314, 1615.734, 985.154, 30);

    strategy->directions[0] = FORWARD;

    strategy->speeds[0] = 9;

    return strategy;
	}
	else if(num == 2 || num == 7)
	{

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


