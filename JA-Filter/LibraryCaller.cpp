//Temat: Filtrowanie uwypuklaj�ce skierowane na zach�d.
//Algorytm b�dzie filtrowa� ka�dy z kolor�w pixeli zgodnie z mask�
//1 0 -1
//1 1 -1
//1 0 -1
//Wykona�: Filip Gaida gr.3 semestr 5, 2020/2021


#include "LibraryCaller.h"
#include "FileInterface.h"
#include <thread>
#include <vector>
#include <array>
#include <queue>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;


void LibraryCaller::LoadFilter()
{
	dllHandle = NULL;
	dllHandle = LoadLibrary(dllName.c_str());
	filter = (FilterProc)GetProcAddress(dllHandle, "filter");
}

void LibraryCaller::UnloadFilter()
{
	filter = nullptr;
	FreeLibrary(dllHandle);
}

unsigned int LibraryCaller::GetCPUThreads()
{
	unsigned int cores = std::thread::hardware_concurrency();
	if (cores <= 0) {
		return 1;
	}
	return cores;
}

bool LibraryCaller::CheckAVX2()
{
	int info[4];
	__cpuidex(info, 0, 0);
	int nIds = info[0];

	__cpuidex(info, 0x80000000, 0);
	unsigned nExIds = info[0];

	if (nIds >= 0x00000007) {
		__cpuidex(info, 0x00000007, 0);
		return (info[1] & ((int)1 << 5)) != 0;
	}

	return false;
}

LibraryCaller::LibraryCaller(): 
	inputFile("input.bmp"), 
	outputFile("output.bmp"), 
	dllName(L""), 
	threads(GetCPUThreads()), 
	dllHandle(NULL), 
	filter(nullptr),
	hasAVX2(CheckAVX2())
{
	SetASM();
}

void LibraryCaller::ProcessImage(uint8_t* pixels, uint8_t* newPixels,int imageSize, int h)
{
	//Rzeczywista szeroko�� wiersza w bajtach.
	int realWidth = imageSize / h;
	//Ile bajt�w zostanie przypisane pojedynczemu w�tkowi.
	int threadDefaultSize = ((((h - 2) * realWidth) >> 5) / threads) << 5;
	//Ile bajt�w pozostanie nieprzypisanych.
	//Liczba to ilo�� bajt�w, kt�re nie zmie�ci�y si� w zestawie 32 bajt�w, oraz te kt�re zosta�y przypisane
	//do 32, ale nie zosta�y przypisane do w�tku. Zostan� rodzdzielone pomi�dzy w�tki.
	int remaining = (((h - 2) * realWidth)) % 32 + (((((h - 2) * realWidth) >> 5) % threads) << 5);
	//Pomini�cie dolnej kraw�dzi przez uznanie jej za przetworzonej.
	int processedSize = realWidth; 

	std::vector<thread> threadsContainer;
	for (unsigned int i = 0; i < threads; i += 1) {
		int threadDataSize = threadDefaultSize;
		if (remaining > 0) {
			//Gdy zostaj� dodatkowe bajty, przypisz dodatkowe 32 lub mniej bajt�w najstarszm w�tkom.
			if (remaining >= 32) {
				threadDataSize += 32;
			}
			else {
				threadDataSize += remaining;
			}
			remaining-=32;
		}

		threadsContainer.push_back(move(thread(filter,
			pixels + processedSize, //tablica przesuni�ta o ilo�� danych przetworzonych przez poprzedni w�tek.
			newPixels + processedSize, //przesuni�te podobnie jak poprzednia tablica.
			threadDataSize,
			realWidth)));

		//Uznaj dane przetwarzane przez w�tek jako przetworzone.
		processedSize += threadDataSize;
	}
	for (auto& thr : threadsContainer) {
		thr.join();
	}

}

bool LibraryCaller::Run()
{
	//inicializacja zmiennych
	int result = true;
	uint8_t* pixels = nullptr;
	uint8_t* newPixels = nullptr;
	BITMAPFILEHEADER* bmpHeader = nullptr;
	BITMAPINFOHEADER* bmpInfo = nullptr;
	lastRunDurration = std::chrono::microseconds(0);
	lastHeight = 0;
	lastWidth = 0;
	//Za�adowanie biblioteki
	LoadFilter();
	if (ReadBMP(inputFile.c_str(), pixels, bmpHeader, bmpInfo)) {
		newPixels = new uint8_t[bmpInfo->biSizeImage]; //alokacja tablicy wynikowej.
		auto start = std::chrono::steady_clock().now(); //pocz�tek pomiaru czasu.
		ProcessImage(pixels, newPixels, bmpInfo->biSizeImage, bmpInfo->biHeight);
		auto end = std::chrono::steady_clock().now(); //koniec pomiaru czasu.
		lastRunDurration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		if (!SaveBMP(outputFile.c_str(), newPixels, bmpHeader, bmpInfo)) {
			result = false;
		}
		lastHeight = bmpInfo->biHeight;
		lastWidth = bmpInfo->biWidth;
	}
	else {
		result = false;
	}
	if(pixels)
		delete[] pixels;
	if (newPixels)
		delete[] newPixels;
	if (bmpHeader)
		delete bmpHeader;
	if (bmpInfo)
		delete bmpInfo;
	//Zwolnienie biblioteki
	UnloadFilter();
	//Przekazanie u�ytkownikowi statystyk.
	SavePerformance();
	ShowPerformance();
	return result;
}

bool LibraryCaller::SavePerformance()
{
	wfstream performanceFile("stats.csv", fstream::in | fstream::out | fstream::app);
	if (!performanceFile) {
		cout << "Nie udalo sie zapisa� pliku z statystykami";
		return false;
	}
	performanceFile << dllName << ";"
		<< threads << ";"
		<< lastWidth << ";"
		<< lastHeight << ";"
		<<lastRunDurration.count() << ";\n";
	performanceFile.close();
	return true;
}

void LibraryCaller::ShowPerformance()
{
	wcout << "Biblioteka: "<<dllName << " "
		<< "Watki: " << threads << " "
		<< "wysokosc: " << lastWidth << " "
		<< "szerokosc: " << lastHeight << " "
		<< "czas(mikrosekundy): " << lastRunDurration.count() << ";\n";
}

ParseCode LibraryCaller::ParseArgs(const std::string& args)
{
	std::stringstream ss(args);
	std::string buffer;
	while (ss >> buffer) {
		if (buffer == "-asm")
			SetASM();
		else if (buffer == "-cpp")
			SetCPP();
		else if (buffer == "-thr") {
			int newThreads = 0;
			if (ss >> newThreads)
				SetThreads(newThreads);
			else
				return ValueMiss;
		}
		else if (buffer == "-i") {
			std::string newInput;
			if (ss >> newInput)
				inputFile = newInput;
			else
				return ValueMiss;
		}
		else if (buffer == "-o") {
			std::string newOutput;
			if (ss >> newOutput)
				outputFile = newOutput;
			else
				return ValueMiss;
		}
		else if (buffer == "-exit")
			return Exit;
	}
	return Passed;
}

void LibraryCaller::SetThreads(int newThreads)
{
	threads = newThreads;
	if (newThreads <= 0) {
		threads = GetCPUThreads();
	}
}

void LibraryCaller::SetASM()
{
	if(hasAVX2)
		dllName = L"ASMFilter.dll";
	else 
		SetCPP(); //Gdy rozkazy niedost�pne skorzystaj z c++.
}

void LibraryCaller::SetCPP()
{
	dllName = L"CPPFilter.dll";
}
