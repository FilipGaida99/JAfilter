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
	int cores = std::thread::hardware_concurrency();
	if (cores <= 0) {
		return 1;
	}
	return cores;
}

LibraryCaller::LibraryCaller(): inputFile("input.bmp"), outputFile("output.bmp"), dllName(L"ASMFilter.dll"), threads(GetCPUThreads()), dllHandle(NULL), filter(nullptr)
{
}

void LibraryCaller::ProcessImage(uint8_t* pixels, uint8_t* newPixels,int w, int h)
{
	LoadFilter();

	int padding = 0;
	if (3*w % 4 != 0) {
		padding = 4 - (3*w) % 4;
	}
	int realWidth = 3 * w + padding;
	int size = (h-2) * realWidth; //Rozmiar jest mniejszy o dwa wiersze, poniewa¿ nie jest prztwarzana krawêdŸ.
	int threadSize = size / threads;
	int remaining = size % threads;
	int processedSize = 0;
	std::vector<thread> threadsContainer;
	for (int i = 0; i < threads; i += 1) {
		int threadDataSize = threadSize;
		if (remaining > 0) {
			threadDataSize+=3;
			remaining--;
		}

		threadsContainer.push_back(move(thread(filter,
			pixels + (processedSize + realWidth), //tablica przesuniêta o jeden wiersz i iloœæ danych przetworzonych przez poprzedni w¹tek.
			newPixels + (processedSize + realWidth), //przesuniête podobnie jak poprzednia tablica
			threadDataSize,
			realWidth)));

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
		ProcessImage(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
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
	if (threads <= 0) {
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
