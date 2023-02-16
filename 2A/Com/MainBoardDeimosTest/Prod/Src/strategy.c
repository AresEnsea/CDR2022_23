/*
 * strategy.c
 */


#include "strategy.h"

Strategy* strategy_initialize(int num)
{
	Strategy* out = strategy_new(2);
	strategy_set_point(out, 0, bezier_new(1075.705, 723.906, 1311.759, 644.217, 1822.838, 521.769, 2063.751, 764.723, 30), FORWARD, 600);
	strategy_set_point(out, 1, bezier_new(2063.751, 764.723, 2304.665, 1007.677, 1251.215, 1113.703, 1095.724, 1251.701, 30), FORWARD, 600);
	return out;
}
