#pragma once
#include "flow_shop.h"
#include <queue>
#include "BlockedOperation.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#define MAX_ITERATIONS 500
#define TABU_SIZE 12

class tabu_search
{
private:
	flow_shop bestPermutation;
	flow_shop currPermutation;
	flow_shop tmpPerm;
	std::queue<std::pair<int,int> > possibleSwaps;
	int bestCmax;
	std::vector<BlockedOperation > tabuList;
public:
	tabu_search();
	~tabu_search();
	bool compareCmaxes(int, int);
	int getBestCmax();
	flow_shop &getCurrPermutation();
	flow_shop &getBestPermutation();
	bool onTabuList(const BlockedOperation &);
	void preparePermutation();
	void mainAlgorithm();
};

