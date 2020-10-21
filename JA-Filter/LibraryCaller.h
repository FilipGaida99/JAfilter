#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
//WskaŸnik na funkcjê filtruj¹c¹ znajduj¹c¹ siê w bibliotece do³¹czanej dynamicznie.
typedef int(_fastcall* FilterProc)(uint8_t* pixels, uint8_t* newPixels, int dataSize, int w);
//Informacja zwrotna z procesu przetwarzania argumentów wejœciowych
enum ParseCode{Passed, ValueMiss, Exit};
//Klasa przechowuj¹ca dane potrzebne do uruchomienia funkcji filtrowania, oraz obs³uguj¹ca ten proces.
class LibraryCaller {
	std::string inputFile; //Nazwa pliku wejœciowego
	std::string outputFile; //Nazwa pliku wyjœciowego
	std::wstring dllName; //Nazwa biblioteki z której bêdzie korzysta³ program
	unsigned int threads; //UStawiona liczba w¹tków.
	HINSTANCE dllHandle; //Uchwyt dla dynamicznie linkowanej biblioteki.
	FilterProc filter; //WskaŸnik na funkcjê filtruj¹c¹

	std::chrono::microseconds lastRunDurration; //Czas trwania ostatniego filtrowania.
	int lastWidth; //Ostatnia odczytana szerokoœæ.
	int lastHeight; //Ostatnia odczytana wysokoœæ.

	//Za³adowanie biblioteki i funkcji.
	void LoadFilter();
	//Zwolnienie biblioteki i funkcji.
	void UnloadFilter();
	//Zwraca oblicza odpowiedni¹ liczbê w¹tków. Gdy komputer nie podaje tych danych, ustawia 1.
	unsigned int GetCPUThreads();
public:
	//Konstruktor.
	LibraryCaller();

	//Funkcja tworz¹ca w¹tki, w których bêzie przetwarzana funkcja filtruj¹ca.
	void ProcessImage(uint8_t* pixels, uint8_t* newPixels, int imageSize, int h);
	//Uruchomienie procesu filtracji na podstawie ustawionych pól obiektu.
	bool Run();
	//Zapis pliku z osi¹gami.
	bool SavePerformance();
	//Wyœwietlenie osi¹gów.
	void ShowPerformance();
	//Zmiana pól opisuj¹cych sposób uruchomienia na podstawie argumentów linii poleceñ podanych przez u¿ytkownika.
	ParseCode ParseArgs(const std::string& args);
	//Ustawia liczbê w¹tków. Gdy mniejsza lub równa 0, ustawia optymaln¹ liczbê w¹tków.
	void SetThreads(int newThreads);
	//Ustawia jako u¿ywan¹ bibliotekê asemblerow¹.
	void SetASM();
	//Ustawia jako u¿ywan¹ bibliotekê napisan¹ w c++.
	void SetCPP();
};