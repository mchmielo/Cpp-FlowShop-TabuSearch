#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include "tabu_search.h"

using namespace std;

vector<string> readFileNames(string file){
	vector<string> fileNames;
	string fileName;
	fstream inFile;
	int count = 0, fileNumber;
	inFile.open(file.c_str(), ios::in);
	if (!inFile.is_open()){
		return fileNames;
	}
	while (!inFile.eof()){
		if (count){
			inFile >> fileNumber;
		}
		else{
			inFile >> fileName;
			fileNames.push_back(fileName);
		}
	}
	inFile.close();
	return fileNames;
}

int main()
{
	tabu_search ts;

	unsigned __int64 freq, counterStart, counterStop;	// zmienne do mierzenia czasu dzialania 
	long double timer;

	srand(static_cast<unsigned int>(time(NULL)));
	vector<string> fileNames = readFileNames("Data/names.txt");
	if (!fileNames.empty()){
		for (int i = 0; i < static_cast<int>(fileNames.size()); ++i){
			if (i){
				timer = 0;
				string fName = "Data/";
				fName += fileNames[i];
				QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*> (&freq));
				QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*> (&counterStart));	// wystartowanie pomiaru czasu

				ts.getCurrPermutation().clearFlowShop();

				ts.getCurrPermutation().readFile(fName);
				ts.preparePermutation();
				ts.getCurrPermutation().createHTMLFile(fileNames[i]+string(".html"));
				ts.getCurrPermutation().logClass();
				//ts.mainAlgorithm();

				QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*> (&counterStop));	// koniec pomiaru czasu
				timer = (static_cast<long double> (counterStop)-counterStart) / freq;
				cout << fName << ", Cmax = " << ts.getBestCmax() << ",  czas[s] = " << timer << endl;
			}
		}
	}
	system("pause");
	return 0;
}

