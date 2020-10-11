#include "LibraryCaller.h"
#include "FileInterface.h"
#include <thread>
#include <vector>
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
	return std::thread::hardware_concurrency();
}

LibraryCaller::LibraryCaller(): inputFile("input.bmp"), outputFile("output.bmp"), dllName(L"ASMFilter.dll"), threads(GetCPUThreads()), dllHandle(NULL), filter(nullptr)
{
}

void LibraryCaller::ProcessImage(uint8_t* pixels, uint8_t* newPixels,int w, int h)
{
	LoadFilter();

	queue<thread> threadStack;
	for (int i = 1 * w; i < (h - 1) * w; i += w) {
		if (threadStack.size() >= threads) {
			threadStack.front().join();
			threadStack.pop();
		}
		threadStack.push(move(thread(filter, pixels, newPixels, i, w)));
	}
	while (!threadStack.empty()) {
		threadStack.front().join();
		threadStack.pop();
	}

	UnloadFilter();
}

void LibraryCaller::Run()
{
	uint8_t* pixels = nullptr;
	BITMAPFILEHEADER* bmpHeader = nullptr;
	BITMAPINFOHEADER* bmpInfo = nullptr;
	ReadBMP(inputFile.c_str(), pixels, bmpHeader, bmpInfo);
	uint8_t* newPixels = new uint8_t[bmpInfo->biSizeImage];
	ProcessImage(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
	SaveBMP(outputFile.c_str(), newPixels, bmpHeader, bmpInfo);

	if(pixels)
		delete[] pixels;
	if (newPixels)
		delete[] newPixels;
	if (bmpHeader)
		delete bmpHeader;
	if (bmpInfo)
		delete bmpInfo;
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
