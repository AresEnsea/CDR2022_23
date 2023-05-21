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

    strategy->path[0] = bezier_new(1908.688, 253.753, 1865.272, 544.973, 1779.838, 690.077, 1338.819, 702.890, 30);
    strategy->path[1] = bezier_new(1338.819, 702.890, 897.799, 715.704, 709.494, 385.286, 554.422, 240.759, 30);
    strategy->path[2] = bezier_new(554.422, 240.759, 399.350, 96.231, 244.430, 12.772, 166.511, 3.935, 30);
    strategy->path[3] = bezier_new(166.511, 3.935, 88.592, -4.901, 567.439, 38.071, 720.588, 158.195, 30);
    strategy->path[4] = bezier_new(720.588, 158.195, 873.738, 278.319, 1912.359, 861.121, 1908.640, 253.678, 30);

    strategy->directions[0] = FORWARD;
    strategy->directions[1] = FORWARD;
    strategy->directions[2] = FORWARD;
    strategy->directions[3] = BACKWARD;
    strategy->directions[4] = BACKWARD;

    strategy->speeds[0] = 1000;
    strategy->speeds[1] = 1000;
    strategy->speeds[2] = 1000;
    strategy->speeds[3] = 1000;
    strategy->speeds[4] = 1000;



    return strategy;
}
