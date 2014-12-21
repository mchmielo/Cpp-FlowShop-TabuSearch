#pragma once
#include <string>
#include <queue>

class flow_shop
{
private:
	int n;      /** liczba operacji*/
	int z;      /** liczba zada� */
	int s;		/** liczba stanowisk*/
	int m;      /** liczba maszyn */
	int *czasi; /** czas wykonywania i-tej operacji */
	int *pi;    /** struktura przechowujaca permutacje: pi = [0 operacje_maszyny_1 0 operacje_maszyny_2 0 ... 0] */
	int *ti;    /** nast�pniki technologiczne */
	int *T;		/** poprzedniki technologiczne potrzebne do wyliczenia ci*/
	int *ps;    /** pozycje i-tych operacji w tabeli permutacji */
	int *lp;    /** poprzedniki */
	int *ci;    /** czas zakonczenia i-tej operacji */
	int *ph;	/** tablica pomocnicza w wyznaczaniu �cie�ki krytycznej*/
	int *cPath;	/** �cie�ka krytyczna: [liczba_elementow 0 1.element 2.element ... i.element 0]*/
	int *cPathColor;	/** tablica zapami�tuj�ca pocz�tek i koniec bloku: -1 pocz�tek bloku, 1 koniec bloku */

	int *mFirstPos;		/** tablica zapami�tuje indeksy pierwszych pozycji na maszynach*/
	int *mCount;		/** tablica pami�ta ile operacji jest na danej maszynie*/
//	std::queue < std::pair <int, int> > possibleSwaps; /** kolejka wszystkich mozliwych przestawien [sk�d][dok�d]*/
	int blockSwaps;
	int cMax;
public:
	flow_shop();
	~flow_shop();
	void clearFlowShop();
	/*!
	* \brief Metoda wczytuj�ca dane z pliku
	* Wczytywane dane s� wpisywane do tablicy pi jako dopuszczalna permutacja problemu.
	* Wype�nione zostaj� r�wnie� tablice ps, ti oraz czasi.
	* \param file - nazwa pliku do wczytania
	* \return false je�li nie uda�o si� otworzy� pliku, true w przeciwnym przypadku
	*/
	bool readFile(const std::string &file = "ta01.txt");
	/*!
	* \brief Metoda tworz�ca obiekt
	* Na podstawie wczytanej wcze�niej liczby zada� i maszyn, zostaje zaalokowana pami�� dla
	* tabli pi, ps, ti oraz czasi.
	*/
	void createPi();
	/*!
	* \brief Metoda tworzy tablic� nast�pnik�w technologicznych
	* Do dzia�ania potrzebuje permutacji pi i wype�nionej tablicy ps.
	*/
	void makeTi();
	/*!
	* \brief Metoda tworzy tablice liczba poprzednikow
	* Tablica ta jest niezb�dna dla algorytmu tworz�cego
	* harmonogram, zlicza ile operacji jest gotowych w danym momencie
	* wykonywania algorytmu.
	*/
	void makeLp();
	/*!
	* \brief Metoda s�u�y do wypisywania wszystkich zmiennych klasy.
	* Zmienne te wypisywane s� na stdout.
	*/
	void logClass();
	/*!
	* \brief Obliczanie �cie�ki krytycznej
	*/
	void evalPH(int i);
	/*!
	* \brief Metoda pomocnicza funkcji job_shop::createSchedule().
	* Oblicza czas zako�czenia ::i -tej operacji: c_max(i) = max(T[i-1],pi[ps[i]-1])+czas_wykonywania
	* gdzie T[i-1] to poprzednik technologiczny, pi[ps[i]-1] to poprzednik maszynowy.
	* \param i  - numer operacji
	*/
	void evalCi(int i);
	/*!
	* \brief Metoda tworz�ca harmonogram dla danej permutacji.
	*/
	void createSchedule();
	void splitFirstPermutation();
	int findMaxCi();
	void createCPath();
	void createBlocks();
	void swapPosInPi(int from, int to);
	int findMachine(int);
	void findAllPossibleSwaps(std::queue<std::pair<int,int> > &);
	void copyPermutation(const flow_shop&);

	int getCmax();
	int getBlockSwaps();

	bool isInCPath(int);
	int findFirstOperationOnMachine(int , int);
	void createHTMLFile(const std::string &file = "result.html");
};

