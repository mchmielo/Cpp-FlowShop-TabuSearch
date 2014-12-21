#pragma once
#include <string>
#include <queue>

class flow_shop
{
private:
	int n;      /** liczba operacji*/
	int z;      /** liczba zadañ */
	int s;		/** liczba stanowisk*/
	int m;      /** liczba maszyn */
	int *czasi; /** czas wykonywania i-tej operacji */
	int *pi;    /** struktura przechowujaca permutacje: pi = [0 operacje_maszyny_1 0 operacje_maszyny_2 0 ... 0] */
	int *ti;    /** nastêpniki technologiczne */
	int *T;		/** poprzedniki technologiczne potrzebne do wyliczenia ci*/
	int *ps;    /** pozycje i-tych operacji w tabeli permutacji */
	int *lp;    /** poprzedniki */
	int *ci;    /** czas zakonczenia i-tej operacji */
	int *ph;	/** tablica pomocnicza w wyznaczaniu œcie¿ki krytycznej*/
	int *cPath;	/** œcie¿ka krytyczna: [liczba_elementow 0 1.element 2.element ... i.element 0]*/
	int *cPathColor;	/** tablica zapamiêtuj¹ca pocz¹tek i koniec bloku: -1 pocz¹tek bloku, 1 koniec bloku */

	int *mFirstPos;		/** tablica zapamiêtuje indeksy pierwszych pozycji na maszynach*/
	int *mCount;		/** tablica pamiêta ile operacji jest na danej maszynie*/
//	std::queue < std::pair <int, int> > possibleSwaps; /** kolejka wszystkich mozliwych przestawien [sk¹d][dok¹d]*/
	int blockSwaps;
	int cMax;
public:
	flow_shop();
	~flow_shop();
	void clearFlowShop();
	/*!
	* \brief Metoda wczytuj¹ca dane z pliku
	* Wczytywane dane s¹ wpisywane do tablicy pi jako dopuszczalna permutacja problemu.
	* Wype³nione zostaj¹ równie¿ tablice ps, ti oraz czasi.
	* \param file - nazwa pliku do wczytania
	* \return false jeœli nie uda³o siê otworzyæ pliku, true w przeciwnym przypadku
	*/
	bool readFile(const std::string &file = "ta01.txt");
	/*!
	* \brief Metoda tworz¹ca obiekt
	* Na podstawie wczytanej wczeœniej liczby zadañ i maszyn, zostaje zaalokowana pamiêæ dla
	* tabli pi, ps, ti oraz czasi.
	*/
	void createPi();
	/*!
	* \brief Metoda tworzy tablicê nastêpników technologicznych
	* Do dzia³ania potrzebuje permutacji pi i wype³nionej tablicy ps.
	*/
	void makeTi();
	/*!
	* \brief Metoda tworzy tablice liczba poprzednikow
	* Tablica ta jest niezbêdna dla algorytmu tworz¹cego
	* harmonogram, zlicza ile operacji jest gotowych w danym momencie
	* wykonywania algorytmu.
	*/
	void makeLp();
	/*!
	* \brief Metoda s³u¿y do wypisywania wszystkich zmiennych klasy.
	* Zmienne te wypisywane s¹ na stdout.
	*/
	void logClass();
	/*!
	* \brief Obliczanie œcie¿ki krytycznej
	*/
	void evalPH(int i);
	/*!
	* \brief Metoda pomocnicza funkcji job_shop::createSchedule().
	* Oblicza czas zakoñczenia ::i -tej operacji: c_max(i) = max(T[i-1],pi[ps[i]-1])+czas_wykonywania
	* gdzie T[i-1] to poprzednik technologiczny, pi[ps[i]-1] to poprzednik maszynowy.
	* \param i  - numer operacji
	*/
	void evalCi(int i);
	/*!
	* \brief Metoda tworz¹ca harmonogram dla danej permutacji.
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

