#pragma once
#include "flow_shop.h"

class tabu_search
{
private:
	flow_shop prevPermutation;
	flow_shop currPermutation;
	int bestCmax;
public:
	tabu_search();
	~tabu_search();
	bool compareCmaxes(int, int);
	int getBestCmax();
	flow_shop &getCurrPermutation();
	void preparePermutation();
};

