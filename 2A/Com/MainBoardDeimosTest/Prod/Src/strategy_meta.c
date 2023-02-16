/*
 * strategy_meta.c
 *
 *  Created on: 9 févr. 2023
 *      Author: user
 */


#include "strategy.h"

Strategy* strategy_in_use = NULL;
Strategy* strategy_not_in_use = NULL;
int use_new_strategy = 0, moving_new_strategy = 0, writing_new_strategy = 0;

Strategy* strategy_new(int length)
{
	Strategy* strat = (Strategy*)calloc(1, sizeof(Strategy));
	strat->points = (StrategyPoint*)calloc(length, sizeof(StrategyPoint));
	strat->length = length;
	return strat;
}
void strategy_delete(Strategy* strat)
{
	if(strat == NULL)
		return;

	for(int i = 0; i < strat->length; ++i)
	{
		bezier_delete(strat->points[i].path);
		free(strat->points[i].actions);
	}

	free(strat);
}

uint8_t * strategy_new_actions_list(int length)
{
	return (uint8_t*)calloc(1, length);
}
void strategy_set_point(Strategy* strat, const int point, Bezier* path, Direction direction, float speed)
{
	StrategyPoint* ptr = &strat->points[point];
	ptr->path = path;
	ptr->speed = direction == FORWARD ? speed : -speed;
}
void strategy_set_point_dirspeed(Strategy* strat, const int point, Bezier* path, float speed)
{
	StrategyPoint* ptr = &strat->points[point];
	ptr->path = path;
	ptr->speed = speed;
}
void strategy_set_actions_at_point(Strategy* strat, const int point, uint8_t* actions, int length)
{
	StrategyPoint* ptr = &strat->points[point];
	ptr->actions = actions;
	ptr->actionsLength = length;
}
void strategy_get_point(Strategy* strat, const int point, Bezier** path, Direction* direction, float* speed)
{
	StrategyPoint* ptr = &strat->points[point];
	if(path)
		*path = ptr->path;
	if(direction)
		*direction = ptr->speed < 0.0f ? BACKWARD : FORWARD;
	if(speed)
		*speed = fabsf(ptr->speed);
}
