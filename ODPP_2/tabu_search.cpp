#include "tabu_search.h"
#include <Windows.h>
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
	bestCmax = 2147483647;
	tabuList.assign(TABU_SIZE, BlockedOperation(0, 0, 0));
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

bool tabu_search::onTabuList(const BlockedOperation &currPair){
	
	for (unsigned int i = 0; i < tabuList.size(); ++i){
		if (tabuList[i].isEqual(currPair))
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
	BlockedOperation bO1, bO2;
	std::pair<int, int> currPair, bestPair;
	std::vector<BlockedOperation >::iterator iter;
	bestCmax = 2147483647;
	tabuList.assign(TABU_SIZE, BlockedOperation(0, 0, 0));
	preparePermutation();
	currPermutation.createHTMLFile(std::string("tmp.html"));
	for (int i = 0; i < MAX_ITERATIONS; ++i){
		bestPair = std::make_pair(0, 0);
		int localCmax = 2147483647;
//		currPermutation.createHTMLFile(std::string("tmp") + std::to_string(i) + std::string(".html"));
		tmpPerm.copyPermutation(currPermutation);
		while (!possibleSwaps.empty()){			// sprawdzenie lokalnego cmaxu dla ka¿dego mozliwego ruchu
			currPair = possibleSwaps.front();	// zdjecie pary z kolejki
			possibleSwaps.pop();
			
			
			currPermutation.swapPosInPi(currPair.first, currPair.second);	// zamiana pozycji
			currPermutation.makeBlockedOperationsFromPos(currPair, bO1, bO2);
			

			currPermutation.makeLp();
			currPermutation.createSchedule();								// obliczenie wszystkich ci
			currPermutation.findMaxCi();

			if (!onTabuList(bO1) && !onTabuList(bO2)){					// jeœli nie ma zamiany w tablicy tabu
				
				if (localCmax >= currPermutation.getCmax()){						// zapamietanie najlepszego ruchu i cmaxu
					localCmax = currPermutation.getCmax();
					bestPair = currPair;
				}
				
			}
			else{
				if (currPermutation.getCmax() < bestCmax){						// zapamietanie najlepszego ruchu i cmaxu
					bestCmax = currPermutation.getCmax();
					bestPermutation.copyPermutation(currPermutation);
					localCmax = currPermutation.getCmax();
					bestPair = currPair;
				}
			}
			currPermutation.swapPosInPi(currPair.second, currPair.first);
			currPermutation.revertChangesInPermutation(tmpPerm);

		}
		
		currPermutation.swapPosInPi(bestPair.first, bestPair.second);
		currPermutation.makeLp();
		preparePermutation();
		iter = tabuList.begin();
		currPermutation.makeBlockedOperationsFromPos(bestPair, bO1, bO2);
		
		tabuList.insert(iter, bO1);
		if (!bO2.isEqual(BlockedOperation(-1, -1, -1))){				// jeœli przenosimy na druga maszyne i bO2 tez jest istotne
			iter = tabuList.begin();
			tabuList.insert(iter, bO2);
		}
		while (tabuList.size() > TABU_SIZE)
			tabuList.pop_back();
		if (localCmax < bestCmax){
			bestCmax = localCmax;
			bestPermutation.copyPermutation(currPermutation);
		}
		
	}
}