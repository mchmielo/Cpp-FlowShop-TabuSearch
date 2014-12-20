#include "tabu_search.h"
#include <limits>
#include <string>

tabu_search::tabu_search()
{
	bestCmax = std::numeric_limits<int>::max();
	tabuList.assign(TABU_SIZE, std::make_pair(0, 0));
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

bool tabu_search::onTabuList(const std::pair<int, int> &currPair){
	std::pair<int, int> swappedPair;
	swappedPair.first = currPair.second;
	swappedPair.second = currPair.first;
	for (unsigned int i = 0; i < tabuList.size(); ++i){
		if (tabuList[i] == currPair || tabuList[i] == swappedPair)
			return true;
	}
	return false;
}

void tabu_search::preparePermutation(){
	
	currPermutation.createSchedule();
	currPermutation.createCPath();
	currPermutation.createBlocks();
	currPermutation.findAllPossibleSwaps(possibleSwaps);
}

void tabu_search::mainAlgorith(){
	std::pair<int, int> currPair, bestPair;
	std::vector<std::pair<int, int> >::iterator iter;
	for (int i = 0; i < MAX_ITERATIONS; ++i){
		bestPair = std::make_pair(0, 0);
		int localCmax = std::numeric_limits<int>::max();
		currPermutation.makeLp();
		preparePermutation();
		currPermutation.createHTMLFile(std::string("tmp") + std::to_string(i) + std::string(".html"));
		currPermutation.createHTMLFile(std::string("tmp.html"));
		while (!possibleSwaps.empty()){			// sprawdzenie lokalnego cmaxu dla ka¿dego mozliwego ruchu
			currPair = possibleSwaps.front();	// zdjecie pary z kolejki
			possibleSwaps.pop();
			if (!onTabuList(currPair)){					// jeœli nie ma zamiany w tablicy tabu
				currPermutation.swapPosInPi(currPair.first, currPair.second);	// zamiana pozycji
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci

				currPermutation.findMaxCi();
				if (localCmax > currPermutation.getCmax()){						// zapamietanie najlepszego ruchu i cmaxu
					localCmax = currPermutation.getCmax();
					bestPair = currPair;
				}
				currPermutation.swapPosInPi(currPair.second, currPair.first);	// przywrócenie permutacji wstêpnej
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci
			}

		}
		if (localCmax < bestCmax){
			bestCmax = localCmax;
			iter = tabuList.begin();
			tabuList.insert(iter, bestPair);
			tabuList.pop_back();
			currPermutation.swapPosInPi(bestPair.first, bestPair.second);
		}
		else{
			if (!tabuList.empty())
				tabuList.pop_back();
		}
		
	}
}