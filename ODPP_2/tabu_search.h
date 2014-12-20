#pragma once
#include "flow_shop.h"
#include <queue>
#define MAX_ITERATIONS 1000
#define TABU_SIZE 14

class tabu_search
{
private:
	flow_shop prevPermutation;
	flow_shop currPermutation;
	std::queue<std::pair<int, int> > possibleSwaps;
	int bestCmax;
	std::vector<std::pair<int, int> > tabuList;
public:
	tabu_search();
	~tabu_search();
	bool compareCmaxes(int, int);
	int getBestCmax();
	flow_shop &getCurrPermutation();
	bool onTabuList(const std::pair<int, int> &);
	void preparePermutation();
	void mainAlgorith();
};

