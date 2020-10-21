#pragma once
#include <string>
#include <Windows.h>
#include <chrono>
//Wska�nik na funkcj� filtruj�c� znajduj�c� si� w bibliotece do��czanej dynamicznie.
typedef int(_fastcall* FilterProc)(uint8_t* pixels, uint8_t* newPixels, int dataSize, int w);
//Informacja zwrotna z procesu przetwarzania argument�w wej�ciowych
enum ParseCode{Passed, ValueMiss, Exit};
//Klasa przechowuj�ca dane potrzebne do uruchomienia funkcji filtrowania, oraz obs�uguj�ca ten proces.
class LibraryCaller {
	std::string inputFile; //Nazwa pliku wej�ciowego
	std::string outputFile; //Nazwa pliku wyj�ciowego
	std::wstring dllName; //Nazwa biblioteki z kt�rej b�dzie korzysta� program
	unsigned int threads; //UStawiona liczba w�tk�w.
	HINSTANCE dllHandle; //Uchwyt dla dynamicznie linkowanej biblioteki.
	FilterProc filter; //Wska�nik na funkcj� filtruj�c�

	std::chrono::microseconds lastRunDurration; //Czas trwania ostatniego filtrowania.
	int lastWidth; //Ostatnia odczytana szeroko��.
	int lastHeight; //Ostatnia odczytana wysoko��.

	//Za�adowanie biblioteki i funkcji.
	void LoadFilter();
	//Zwolnienie biblioteki i funkcji.
	void UnloadFilter();
	//Zwraca oblicza odpowiedni� liczb� w�tk�w. Gdy komputer nie podaje tych danych, ustawia 1.
	unsigned int GetCPUThreads();
public:
	//Konstruktor.
	LibraryCaller();

	//Funkcja tworz�ca w�tki, w kt�rych b�zie przetwarzana funkcja filtruj�ca.
	void ProcessImage(uint8_t* pixels, uint8_t* newPixels, int imageSize, int h);
	//Uruchomienie procesu filtracji na podstawie ustawionych p�l obiektu.
	bool Run();
	//Zapis pliku z osi�gami.
	bool SavePerformance();
	//Wy�wietlenie osi�g�w.
	void ShowPerformance();
	//Zmiana p�l opisuj�cych spos�b uruchomienia na podstawie argument�w linii polece� podanych przez u�ytkownika.
	ParseCode ParseArgs(const std::string& args);
	//Ustawia liczb� w�tk�w. Gdy mniejsza lub r�wna 0, ustawia optymaln� liczb� w�tk�w.
	void SetThreads(int newThreads);
	//Ustawia jako u�ywan� bibliotek� asemblerow�.
	void SetASM();
	//Ustawia jako u�ywan� bibliotek� napisan� w c++.
	void SetCPP();
};