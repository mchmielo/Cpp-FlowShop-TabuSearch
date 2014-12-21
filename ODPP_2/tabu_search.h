#pragma once
#include "flow_shop.h"
#include <queue>
#define MAX_ITERATIONS 50
#define TABU_SIZE 10

class tabu_search
{
private:
	flow_shop bestPermutation;
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
	flow_shop &getBestPermutation();
	bool onTabuList(const std::pair<int, int> &);
	void preparePermutation();
	void mainAlgorith();
};

