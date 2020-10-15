#include "LibraryCaller.h"
#include "FileInterface.h"
#include <thread>
#include <vector>
#include <array>
#include <queue>
#include <sstream>

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

LibraryCaller::LibraryCaller(): inputFile("input.bmp"), outputFile("output.bmp"), dllName(L"ASMFilter.dll"), threads(GetCPUThreads()), dllHandle(NULL), filter(nullptr)
{
}

void LibraryCaller::ProcessImage(uint8_t* pixels, uint8_t* newPixels,int imageSize, int h)
{
	LoadFilter();
	//Rzeczywista szeroko�� wiersza w bajtach.
	int realWidth = imageSize / h;
	//Ile bajt�w zostanie przypisane pojedynczemu w�tkowi.
	int threadDefaultSize = ((((h - 2) * realWidth) >> 4) / threads) << 4;
	//Ile bajt�w pozostanie nieprzypisanych.
	//Liczba to ilo�� bajt�w, kt�re nie zmie�ci�y si� w zestawie 16 bajt�w, oraz te kt�re zosta�y przypisane
	//do 16, ale nie zosta�y przypisane do w�tku. Zostan� rodzdzielone pomi�dzy w�tki.
	int remaining = (((h - 2) * realWidth)) % 16 + (((((h - 2) * realWidth) >> 4) % threads) << 4);
	//Pomini�cie dolnej kraw�dzi przez uznanie jej za przetworzonej.
	int processedSize = realWidth; 

	std::vector<thread> threadsContainer;
	for (unsigned int i = 0; i < threads; i += 1) {
		int threadDataSize = threadDefaultSize;
		if (remaining > 0) {
			//Gdy zostaj� dodatkowe bajty, przypisz dodatkowe 16 lub mniej bajt�w najstarszm w�tkom.
			if (remaining >= 16) {
				threadDataSize += 16;
			}
			else {
				threadDataSize += remaining;
			}
			remaining-=16;
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

	UnloadFilter();
}

bool LibraryCaller::Run()
{
	int result = true;
	uint8_t* pixels = nullptr;
	uint8_t* newPixels = nullptr;
	BITMAPFILEHEADER* bmpHeader = nullptr;
	BITMAPINFOHEADER* bmpInfo = nullptr;
	if (ReadBMP(inputFile.c_str(), pixels, bmpHeader, bmpInfo)) {
		newPixels = new uint8_t[bmpInfo->biSizeImage];
		ProcessImage(pixels, newPixels, bmpInfo->biSizeImage, bmpInfo->biHeight);
		if (!SaveBMP(outputFile.c_str(), newPixels, bmpHeader, bmpInfo)) {
			result = false;
		}
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
	return result;
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
	dllName = L"ASMFilter.dll";
}

void LibraryCaller::SetCPP()
{
	dllName = L"CPPFilter.dll";
}
