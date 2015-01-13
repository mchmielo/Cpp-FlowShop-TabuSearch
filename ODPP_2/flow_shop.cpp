#include "flow_shop.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip>


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;

flow_shop::flow_shop()
{
	pi = NULL;
	ti = NULL;
	ps = NULL;
	czasi = NULL;
	ci = NULL;
	lp = NULL;
	T = NULL;
	ph = NULL;
	cPath = NULL;
	cPathColor = NULL;
	cPathIndexes = NULL;
	mFirstPos = NULL;
	mCount = NULL; 
	n = 0;
	m = 0;
	z = 0;
	blockSwaps = 0;
	cMax = 0;
}


flow_shop::~flow_shop()
{
	clearFlowShop();
}


void flow_shop::clearFlowShop(){
	if (pi != NULL){
		delete pi;
		pi = NULL;
	}
	if (ti != NULL){
		delete ti;
		ti = NULL;
	}
	if (czasi != NULL){
		delete czasi;
		czasi = NULL;
	}
	if (ps != NULL){
		delete ps;
		ps = NULL;
	}
	if (lp != NULL){
		delete lp;
		lp = NULL;
	}
	if (ci != NULL){
		delete ci;
		ci = NULL;
	}
	if (T != NULL){
		delete T;
		T = NULL;
	}
	if (ph != NULL){
		delete ph;
		ph = NULL;
	}
	if (cPath != NULL){
		delete cPath;
		cPath = NULL;
	}
	if (cPathColor != NULL){
		delete cPathColor;
		cPathColor = NULL;
	}
	if (cPathIndexes != NULL){
		delete cPathIndexes;
		cPathIndexes = NULL;
	}
	if (mFirstPos != NULL){
		delete mFirstPos;
		mFirstPos = NULL;
	}
	if (mCount != NULL){
		delete mCount;
		mCount = NULL;
	}
	s = 0;
	n = 0;
	m = 0;
	z = 0;
	blockSwaps = 0;
	cMax = 0;
}

void flow_shop::logClass()
{
	cout << endl << "pi: " << endl;
	for (int j = 0; j < this->n + this->m + 1; ++j){
		if (j%(this->z+2) == 0)
			cout << this->pi[j] << endl;
		else cout << this->pi[j] << " ";
	}
	cout << endl << "ps: " << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->ps[j] << " ";
	}
	cout << endl << "ti: " << endl;
	for (int j = 0; j < this->n; ++j){
		cout << this->ti[j] << " ";
	}
	cout << endl << "czasi: " << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->czasi[j] << " ";
	}
	cout << endl << "lp: " << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->lp[j] << " ";
	}
	cout << endl << "ci:" << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->ci[j] << " ";
	}
	cout << endl << "ph:" << endl;
	for (int j = 0; j <= this->n; ++j){
		cout << this->ph[j] << " ";
	}
	cout << endl << endl;
}


bool flow_shop::readFile(const std::string &file){
	fstream inFile;
	int machine = 0;
	inFile.open(file.c_str(), ios::in);
	if (!inFile.is_open()){
		return false;
	}

	int i = 0, row = 1, oper = 0, currPos = 1;

	inFile >> this->z;      // wczytanie liczby zadan
	inFile >> this->s;      // wczytanie liczby stanowisk
	createPi();				// zaalokowanie pamieci na podstawie liczby zadan i maszyn
	mFirstPos[machine++] = 1;
	while (!inFile.eof()){
		oper = row + i*this->s;	// obliczenie numeru obecnej operacji
		inFile >> czasi[oper];	// wczytanie czasu wykonywania obecnej operacji
		this->pi[currPos] = oper;
		this->ps[oper] = currPos;
		currPos++;
		i++;
		
		if (i == this->z){			// wyzerowanie licznika zadan po przekroczeniu row*z
			mFirstPos[machine++] = currPos;
			mFirstPos[machine++] = currPos + 2;
			i = 0;
			row++;						// zwiekszenie licznika wierszy
			currPos += 2;		// zwiêkszenie wskaŸnika obecnej pozycji
		}
	}
//	splitFirstPermutation();
	inFile.close();
	makeTi();
	makeLp();
	return true;
}

void flow_shop::splitFirstPermutation(){
	int machine = 0, index1, index2;
	for (int i = 0; i < this->s; ++i){
		machine = 2 * i;		// 0, 2, 4	- pierwsze maszyny na stanowisku
		index1 = mFirstPos[machine];
		index2 = mFirstPos[machine + 1];
		while (pi[index1] != 0){
			swapPosInPi(index1, index2);
			index1++;
		}
	}
}

void flow_shop::createPi()
{
	this->m = this->s * 2;
	this->n = this->s * this->z;

	this->pi = new int[this->n + this->m + 1];
	this->ti = new int[this->n + 1];
	this->ps = new int[this->n + 1];
	this->czasi = new int[this->n + 1];
	this->lp = new int[this->n + 1];
	this->ci = new int[this->n + 1];
	this->T = new int[this->n + 1];
	this->ph = new int[this->n + 1];
	this->mFirstPos = new int[this->m + 1];
	this->mCount = new int[this->m];

	for (int i = 0; i < this->n + this->m + 1; ++i){
		this->pi[i] = 0;
	}
	for (int i = 0; i <= this->n; ++i){
		this->ti[i] = 0;
		this->lp[i] = 0;
		this->ci[i] = 0;
		this->T[i] = 0;
		this->ph[i] = 0;
	}
	for (int i = 0; i < this->m; ++i){
		if (i % 2 == 0){
			this->mCount[i] = this->z;
		}
		else
			this->mCount[i] = 0;
	}
	this->ps[0] = 0;
	this->czasi[0] = 0;
}

void flow_shop::makeTi()
{
	for (int i = 1; i <= this->n; ++i){
		if ((i - 1) % this->s != 0){
			this->ti[i - 1] = this->pi[this->ps[i]];
		}
		if ((i) % this->s != 0){
			this->T[i] = i;
		}
	}
}

void flow_shop::makeLp()
{
	for (int i = 0; i <= this->n; ++i){
		this->lp[i] = 0;
	}
	int indexes = -1;						// indeksy ktore nalezy w kolejnej petli inkrementowac
	for (int i = this->n; i > 0; --i){		// iteracja od tylu po nastêpnikach technologicznych
		/* nastêpniki technologiczne */
		if (indexes > 0){
			this->lp[indexes]++;
		}
		if (this->ti[i - 1] != 0){
			indexes = this->ti[i - 1];
		}
		else indexes = -1;

		/* nastêpniki maszynowe */

		if (this->pi[this->ps[i] - 1] != 0){
			lp[i]++;            //jeœli element przed danym nie jest 0 to ma poprzednika
		}
	}
	if (indexes > 0){
		this->lp[indexes]++;
	}
}

void flow_shop::evalPH(int i){
	ph[i] = (ci[this->pi[ps[i] - 1]] > ci[T[i - 1]]) ? this->pi[ps[i] - 1] : T[i - 1];
}

void flow_shop::evalCi(int i)
{
	ci[i] = max(ci[this->pi[ps[i] - 1]], ci[T[i - 1]]) + czasi[i];      // poprzednik maszynowy
}

void flow_shop::createSchedule()
{
	queue<int> Q;			// kolejka z gotowymi operacjami
	for (int i = 1; i <= this->n; ++i){
		if (lp[i] == 0){
			Q.push(i);
		}
	}
	while (!Q.empty()){
		int i = Q.front(), ns, count = 0;
		evalCi(i);
		evalPH(i);
		Q.pop();
		ns = ti[i];		//nastepnik technologiczny
		do{
			if (ns != 0){
				lp[ns]--;
				if (lp[ns] == 0)
					Q.push(ns);
			}
			ns = pi[ps[i] + 1];
			count++;
		} while (count < 2);	// trzeba wszystko powtorzyc dla nastepnika maszynowego
	}
}
int flow_shop::findMaxCi(){
	int max = 0, max_i = 0;
	for (int i = 0; i <= this->n; ++i){
		if (ci[i] > max){
			max = ci[i];
			max_i = i;
		}
	}
	cMax = max;
	return max_i;
}

int flow_shop::getCmax(){
	return this->cMax;
}

void flow_shop::createCPath(){
	int count = 1, maxCi = findMaxCi(), tmp;
	tmp = maxCi;
	while (ph[tmp] != 0){				// zliczenie elementow w œcie¿ce krytycznej
		tmp = ph[tmp];
		count++;
	}
	if (this->cPath != NULL)
		delete this->cPath;
	if (this->cPathColor != NULL)
		delete this->cPathColor;
	// alokacja potrzebnej pamieci dla sciezki krytycznej
	this->cPath = new int[count + 4];			// w tym element 0 jest liczb¹ elmentów sk, element drugi i ostatni s¹ zerami
	this->cPathColor = new int[count];		// i blokow
	for (int i = 0; i < count; ++i){
		cPathColor[i] = 0;
	}
	cPath[0] = count;
	cPath[1] = 0;
	cPath[count + 2] = 0;
	cPath[count + 1] = maxCi;
	while (ph[cPath[count + 1]] != 0 && count > 0){		// wyznaczanie œcie¿ki krytycznej
		cPath[count] = ph[cPath[count + 1]];
		count--;
	}
}

void flow_shop::createBlocks(){
	int index = 0;
	blockSwaps = 0;
	bool started = 0, isSide = 0;
	for (int i = 2; i < (cPath[0] + 2); ++i){

		//nie jest ostatni w œcie¿ce   //poprzednik w œcie¿ce jest tech.	//nastêpnik w œcie¿ce jest maszynowy
		if ((cPath[i + 1] != 0) && (cPath[i - 1] == T[cPath[i] - 1]) && (cPath[i + 1] == pi[ps[cPath[i]] + 1])){		// pocz¹tek bloku
			cPathColor[i - 2] = 1;
			blockSwaps++;
			started = 1;
			isSide = 1;
		}
		//nie jest pierwszy w œcie¿ce   //nastêpnik w œcie¿ce jest tech.	//poprzednik w œcie¿ce jest maszynowy
		if ((cPath[i - 1] != 0) && (cPath[i + 1] == ti[cPath[i]]) && (cPath[i - 1] == pi[ps[cPath[i]] - 1])){		// koniec bloku
			cPathColor[i - 2] = -1;
			blockSwaps++;
			started = 0;
			isSide = 1;
		}
		if (started && !isSide){
			cPathColor[i - 2] = 2;		// œrodek bloku
		}
		if (pi[ps[cPath[i]] - 1] == 0 && pi[ps[cPath[i]] + 1] && !isSide){	// jesli brak poprzednika i nastepnika maszynowego 
			cPathColor[i - 2] = 3;						// jedyny element na maszynie (tylko przeniesienie na inna maszyne)
		}
		isSide = 0;
	}
	/*
	if (cPathIndexes != NULL){
		delete cPathIndexes;
	}
	cPathIndexes = new int[blockSwaps];
	for (int i = 0; i < cPath[0]; ++i){
		if (cPathColor[i] != 0)
			cPathIndexes[index++] = i;
	}*/
}

void flow_shop::swapPosInPi(int fromIndex, int toIndex){		//
	int from, fromMachine, toMachine;
	from = pi[fromIndex];
	fromMachine = findMachine(fromIndex)-1;
	toMachine = findMachine(toIndex)-1;
	if (pi[toIndex] == 0 && fromIndex < toIndex && fromMachine == toMachine)
		toMachine++;

	if (fromIndex < toIndex){
		for (int i = fromIndex; i < toIndex; ++i){
			pi[i] = pi[i + 1];
			ps[pi[i]] = i;
		}
		if (fromMachine < toMachine){		// jeœli przeniesiono na druga maszyne
			mCount[fromMachine]--;			// zmniejsz stan na pierwszej
			mCount[toMachine]++;			// zwieksz stan na drugiej
			if (mCount[toMachine] > 1){			// jesli przesunal siê poczatek drugiej maszyny
				mFirstPos[toMachine]--;			// to trzeba go przesunac
			}
		}
		
	}
	else{
		for (int i = fromIndex; i > toIndex; --i){
			pi[i] = pi[i - 1];
			ps[pi[i]] = i;
		}
		if (fromMachine > toMachine){		// jeœli przeniesiono na druga maszyne
			mCount[fromMachine]--;			// zmniejsz stan na pierwszej
			mCount[toMachine]++;			// zwieksz stan na drugiej
			if (mCount[fromMachine] >= 1){			// jesli przesunal siê poczatek drugiej maszyny
				mFirstPos[fromMachine]++;			// to trzeba go przesunac
			}
		}
		
	}
	pi[toIndex] = from;
	ps[from] = toIndex;
}

int flow_shop::findMachine(int index){
	int workspaceNumber = 0;
	workspaceNumber =  static_cast<int>((index-1)/(this->z+2)) + 1;

	// pierwsza maszyna stanowiska
	if (index >= mFirstPos[workspaceNumber * 2 - 2] && index < mFirstPos[workspaceNumber * 2 - 1]){
		return (workspaceNumber * 2 - 1);
	}
	// druga maszyna stanowiska
	return (workspaceNumber * 2);
}

void flow_shop::findAllPossibleSwaps(std::queue<std::pair<int, int> > &possibleSwaps){
	int machine = 0, prevMachine = -1, blockBegin = -1, blockEnd = -1;
	BlockedOperation tmpBO;
	int index, machineOnW = -1;
	if (!possibleSwaps.empty()){			// czyszczenie kolejki
		std::queue<std::pair<int, int> > empty;
		std::swap(possibleSwaps, empty);
	}
	for (int i = 0; i < cPath[0]; ++i){
			if (cPathColor[i] == 1){
				int iter = i+1;
				blockBegin = ps[cPath[i + 2]];		// indeks poczatku bloku na tej maszynie
													// znalezienie indeksu konca bloku na tej maszynie
				while (cPathColor[iter] != -1){
					iter++;
				}
				blockEnd = blockBegin + iter - i;
			}

			machine = findMachine(ps[cPath[i + 2]]) - 1;

					
				machineOnW = 0;			// maszyny 0, 2 ,4 ... (pierwsze na stanowisku - nie potrzebuja przesuwania)
			if (machine % 2 == 0 && mCount[machine+1])		// maszyny numerowane od 0 (dlatego odwrotnie warunek)
				machineOnW = -1;			// drugie maszyny na stanowisku potrzebuja przesuniecie o 1 w prawo

			switch (cPathColor[i]){
			case 1:						// jesli poczatek bloku to za blok
				index = blockEnd;
				while (pi[index] != 0){
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index++));
				}
				break;
			case 2:						// jesli srodek bloku to przed blok i za blok
				index = mFirstPos[machine];
				while (index <= blockBegin){
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index++));
				}
				index = blockEnd;
				while (pi[index] != 0){
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index++));
				}
				break;
			case -1:					// jesli koniec bloku to przed blok
				index = mFirstPos[machine];
				while (index <= blockBegin){
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index++));
				}
				break;
			}

			// dostêpne pozycje na drugiej maszynie - jeœli na nieparzyst¹ to za 0, na parzyst¹ to na 0
			if (machine % 2 != 0){		// jeœli maszyna druga na stanowisku
				index = mFirstPos[machine - 1];
				do{
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index));
				} while (pi[index++] != 0);
			}
			else{						// jesli maszyna pierwsza na stanowisku
				index = mFirstPos[machine + 1] + machineOnW;
				do{
					possibleSwaps.push(make_pair(ps[cPath[i + 2]], index++));
				} while (pi[index] != 0);
			}
	}
}

void flow_shop::swapBlocks(){
	int rnd = (rand() % blockSwaps) + 0;
	int tmp, tmp1, tmp2;
	tmp1 = cPath[cPathIndexes[rnd] + 2];						// losowy element do zamiany
	tmp2 = cPath[cPathIndexes[rnd] + 2 + cPathColor[cPathIndexes[rnd]]];	// nastêpny (pocz¹tek bloku) lub poprzedni (koniec bloku) element
	tmp = pi[ps[tmp1]];							// zamiana elementów w permutacji
	pi[ps[tmp1]] = pi[ps[tmp2]];
	pi[ps[tmp2]] = tmp;
	tmp = ps[tmp1];								// zamiana elementów w ps
	ps[tmp1] = ps[tmp2];
	ps[tmp2] = tmp;
	tmp = czasi[tmp1];							//zamiana czasów
	czasi[tmp1] = czasi[tmp2];
	czasi[tmp2] = tmp;
	for (int i = 0; i < n + 1; ++i){			//przygotowanie pozosta³ych tabel do aktualizacji
		ti[i] = 0;
		T[i] = 0;
		lp[i] = 0;
	}
	makeTi();
	makeLp();
}

void flow_shop::revertChangesInPermutation(const flow_shop &p){
	for (int i = 0; i <= this->n; ++i){
		this->lp[i] = p.lp[i];
		this->ci[i] = p.ci[i];
		this->ph[i] = p.ph[i];
	}
	this->cMax = p.cMax;
}

void flow_shop::copyPermutation(const flow_shop &p){
	this->clearFlowShop();
	
	this->n = p.n;
	this->z = p.z;
	this->s = p.s;
	this->m = p.m;
	this->cMax = p.cMax;
	this->blockSwaps = p.blockSwaps;

	this->pi = new int[this->n + this->m + 1];	//
	this->ti = new int[this->n + 1];			//
	this->ps = new int[this->n + 1];			//
	this->czasi = new int[this->n + 1];			//
	this->lp = new int[this->n + 1];
	this->ci = new int[this->n + 1];
	this->T = new int[this->n + 1];				//
	this->ph = new int[this->n + 1];			
	this->mFirstPos = new int[this->m + 1];
	this->mCount = new int[this->m];

	this->cPath = new int[p.cPath[0] + 4];
	this->cPathColor = new int[p.cPath[0]];

	for (int i = 0; i < this->n + this->m + 1; ++i){
		this->pi[i] = p.pi[i];
	}
	for (int i = 0; i <= this->n; ++i){
		this->czasi[i] = p.czasi[i];
		this->ti[i] = p.ti[i];
		this->ps[i] = p.ps[i];
		this->lp[i] = p.lp[i];
		this->ci[i] = p.ci[i];
		this->ph[i] = p.ph[i];
		this->T[i] = p.T[i];
	}
	for (int i = 0; i < this->m; ++i){
		this->mFirstPos[i] = p.mFirstPos[i];
		this->mCount[i] = p.mCount[i];
	}
	for (int i = 0; i < p.cPath[0] + 4; ++i){
		this->cPath[i] = p.cPath[i];
	}
	for (int i = 0; i < p.cPath[0]; ++i){
		this->cPathColor[i] = p.cPathColor[i];
	}
}

void flow_shop::makeBlockedOperationsFromPos(const std::pair<int, int> position, BlockedOperation &bO1, BlockedOperation &bO2){
	int machine1 = findMachine(position.first);
	int machine2 = findMachine(position.second);
	if (machine1 != machine2){
		bO1.makeBlockedOperation(pi[position.second], pi[position.second - 1], machine1);
		bO1.makeBlockedOperation(pi[position.second], pi[position.second + 1], machine1);
	}
	else{
		if (position.first > position.second){	// w lewo
			bO1.makeBlockedOperation(pi[position.second], pi[position.second - 1], machine1);
		}
		else{		// w prawo
			bO1.makeBlockedOperation(pi[position.second], pi[position.second + 1], machine1);
		}
		bO2.makeBlockedOperation(-1, -1, -1);
	}
}

int flow_shop::Pi(int index){
	return pi[index];
}

int flow_shop::Ps(int index){
	return ps[index];
}

bool flow_shop::isInCPath(int index){
	for (int i = 0; i < cPath[0]; ++i){
		if (index == cPath[i + 2])
			return true;
	}
	return false;
}

int flow_shop::findFirstOperationOnMachine(int workplace, int machine){
	int first = workplace + (workplace - 1) + (workplace - 1)*this->z;	// indeks pierwszej operacji na danym stanowisku
	int index = first;
	if (machine == 0){					// jesli pierwsza maszyna na stanowisku to zwróæ indeks
		return first;
	}
	else{
							// szukaj zera od pierwszego elementu na danym stanowisku
		while (pi[index] != 0){
			index++;
		}
	}
	return index + 1;
}

void flow_shop::createHTMLFile(const std::string &file){
	vector<string> colors;
	colors.push_back("FFCC00"); 
	colors.push_back("66FF66"); 
	colors.push_back("00CCFF");
	colors.push_back("CCFFFF"); 
	colors.push_back("FF6666"); 
	colors.push_back("CCCCCC");
	colors.push_back("CCCC99"); 
	colors.push_back("009999"); 
	colors.push_back("FFCC99");
	colors.push_back("00CC00"); 
	colors.push_back("CF0000"); 
	colors.push_back("3399FF");
	fstream outFile;
	outFile.open(file.c_str(), ios::out);
	if (!outFile.is_open()){
		return;
	}
	outFile << "<html>\n<head>\n<link type = 'text/css' rel = 'stylesheet' href = 'ddd.css' / >\n</head>\n<body>\n<div class = 'chart'>\n";
	for (int i = 0; i < this->s; ++i){				// po stanowiskach
		outFile << "<div class = 'workplace'>\n";
		for (int j = 0; j < 2; ++j){			// po maszynach
			outFile << "<div class='machine'>\n";
			int index = findFirstOperationOnMachine(i+1, j);
			while (pi[index] != 0){
				int tmp = static_cast<int>(ceil(static_cast<double>(pi[index]) / static_cast<double>(this->s)))%colors.size();
				
				if (isInCPath(pi[index])){
					outFile << "<div class='job cPath'";
				}
				else{
					outFile << "<div class='job'";
				}
				outFile << " style='left:" << ci[pi[index]] - czasi[pi[index]] << "px; background: #" << colors[tmp] << "; width:" << czasi[pi[index]] << "px'>" << pi[index] << "<br>" << index << "</div>\n";
				index++;
			}
			outFile << "</div>\n";
		}
		outFile << "</div>\n";
	}
	outFile << "</div>\n";
	outFile << "<div class='xaxis'>";
	for (int i = 0; i < static_cast<int>(cMax / 100); ++i){
		outFile << "<div style='left:" << i*100 <<"px'>" << i*100 << "</div>\n";
	}
	outFile << "</div><div style='white-space:pre'><br>pi:<br> " << endl;
	for (int j = 0; j < this->n + this->m + 1; ++j){
		if (j % (this->z + 2) == 0)
			outFile << this->pi[j] << "<br>" << endl;
		else outFile << this->pi[j] << "\t";
	}
	outFile << "</div>\n"<< "<br>Cmax: " << cMax <<"</body>\n</html>\n";
	outFile.close();
}