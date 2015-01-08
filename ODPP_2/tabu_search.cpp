#include "tabu_search.h"
#include <limits>
#include <string>
#include <iostream>


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
tabu_search::tabu_search()
{
	bestCmax = std::numeric_limits<int>::max();
	tabuList.assign(TABU_SIZE, std::make_pair(0, 0));
}


tabu_search::~tabu_search()
{
}

int tabu_search::getBestCmax(){
	return bestCmax;
}

flow_shop &tabu_search::getCurrPermutation(){
	return currPermutation;
}
flow_shop &tabu_search::getBestPermutation(){
	return bestPermutation;
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

void tabu_search::mainAlgorithm(){
	std::pair<int, int> currPair, bestPair;
	std::vector<std::pair<int, int> >::iterator iter;
	bestCmax = std::numeric_limits<int>::max();
	tabuList.assign(TABU_SIZE, std::make_pair(0, 0));
	preparePermutation();
	currPermutation.createHTMLFile(std::string("tmp.html"));
	for (int i = 0; i < MAX_ITERATIONS; ++i){
		bestPair = std::make_pair(0, 0);
		int localCmax = std::numeric_limits<int>::max();
//		currPermutation.createHTMLFile(std::string("tmp") + std::to_string(i) + std::string(".html"));
		
		while (!possibleSwaps.empty()){			// sprawdzenie lokalnego cmaxu dla ka¿dego mozliwego ruchu
			currPair = possibleSwaps.front();	// zdjecie pary z kolejki
			possibleSwaps.pop();
			if (!onTabuList(currPair)){					// jeœli nie ma zamiany w tablicy tabu
				currPermutation.swapPosInPi(currPair.first, currPair.second);	// zamiana pozycji
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci

				currPermutation.findMaxCi();
				if (localCmax >= currPermutation.getCmax()){						// zapamietanie najlepszego ruchu i cmaxu
					localCmax = currPermutation.getCmax();
					bestPair = currPair;
				}
				currPermutation.swapPosInPi(currPair.second, currPair.first);	// przywrócenie permutacji wstêpnej
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci
			}
			else{
				currPermutation.swapPosInPi(currPair.first, currPair.second);	// zamiana pozycji
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci

				currPermutation.findMaxCi();
				if (currPermutation.getCmax() < bestCmax){						// zapamietanie najlepszego ruchu i cmaxu
					bestCmax = currPermutation.getCmax();
					bestPermutation.copyPermutation(currPermutation);
					localCmax = currPermutation.getCmax();
					bestPair = currPair;
				}
				currPermutation.swapPosInPi(currPair.second, currPair.first);	// przywrócenie permutacji wstêpnej
				currPermutation.makeLp();
				currPermutation.createSchedule();								// obliczenie wszystkich ci
			}

		}
		currPermutation.swapPosInPi(bestPair.first, bestPair.second);
		currPermutation.makeLp();
		preparePermutation();
		iter = tabuList.begin();
		tabuList.insert(iter, bestPair);
		if (tabuList.size() >= TABU_SIZE)
			tabuList.pop_back();
		if (localCmax < bestCmax){
			bestCmax = localCmax;
			bestPermutation.copyPermutation(currPermutation);
		}
		
	}
}