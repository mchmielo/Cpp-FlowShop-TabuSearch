#include "tabu_search.h"
#include <limits>

tabu_search::tabu_search()
{
	bestCmax = std::numeric_limits<int>::max();
}


tabu_search::~tabu_search()
{
}

/*bool tabu_search::compareCmaxes(int prevCmax, int currCmax){
	double rnd = static_cast<double>(rand() % 100);
	if ((currCmax >= prevCmax) || (rnd / 100.0 < (exp(-(currCmax - prevCmax)) / currTemp)))
		return true;
	return false;
}*/

int tabu_search::getBestCmax(){
	return bestCmax;
}

flow_shop &tabu_search::getCurrPermutation(){
	return currPermutation;
}

void tabu_search::preparePermutation(){
	currPermutation.createSchedule();
	currPermutation.createCPath();
	currPermutation.createBlocks();
}