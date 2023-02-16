#ifndef _STRATEGY_
#define _STRATEGY_

#include "propulsion.h"
#include "bezier.h"

typedef struct {
	Bezier* path;
	float speed;
	uint8_t* actions;
	int actionsLength;
} StrategyPoint;
typedef struct {
	int length;
	StrategyPoint* points;
} Strategy;

extern Strategy* strategy_in_use;
extern Strategy* strategy_not_in_use;
extern int use_new_strategy, moving_new_strategy, writing_new_strategy;

Strategy* strategy_new(int length);
void strategy_delete(Strategy* strat);
uint8_t * strategy_new_actions_list(int length);

void strategy_set_point(Strategy* strat, const int point, Bezier* path, Direction direction, float speed);
void strategy_set_point_dirspeed(Strategy* strat, const int point, Bezier* path, float speed);
void strategy_set_actions_at_point(Strategy* strat, const int point, uint8_t* actions, int length);
void strategy_get_point(Strategy* strat, const int point, Bezier** path, Direction* direction, float* speed);

Strategy* strategy_initialize(int num);

#endif
