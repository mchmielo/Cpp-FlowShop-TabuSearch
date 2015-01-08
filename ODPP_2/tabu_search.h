#pragma once
#include "flow_shop.h"
#include <queue>

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define MAX_ITERATIONS 300
#define TABU_SIZE 14

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
	void mainAlgorithm();
};

