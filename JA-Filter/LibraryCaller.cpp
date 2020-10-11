#include "LibraryCaller.h"
#include "FileInterface.h"
#include <thread>
#include <vector>
#include <queue>

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

LibraryCaller::LibraryCaller(): inputName(L""), outputName(L"output.bmp"), dllName(L"ASMFilter.dll"), threads(GetCPUThreads()), dllHandle(NULL), filter(nullptr)
{
}

LibraryCaller& LibraryCaller::instance()
{
	static LibraryCaller instance_;
	return instance_;
}

int LibraryCaller::ProcessImage(uint8_t* pixels, uint8_t* newPixels,int w, int h)
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
	return false;
}

int LibraryCaller::Run()
{
	int result = 0;
	uint8_t* pixels = nullptr;
	uint8_t* newPixels = nullptr;
	BITMAPFILEHEADER* bmpHeader = nullptr;
	BITMAPINFOHEADER* bmpInfo = nullptr;
	if (ReadBMP("test.bmp", pixels, bmpHeader, bmpInfo)) {
		uint8_t* newPixels = new uint8_t[bmpInfo->biSizeImage];
		int i = ProcessImage(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
		if (!SaveBMP("testsave.bmp", newPixels, bmpHeader, bmpInfo)) {
			result = 2;
		}
	}
	else {
		result = 1;
	}
	if(pixels)
		delete[] pixels;
	if(newPixels)
		delete[] newPixels;
	if(bmpHeader)
		delete bmpHeader;
	if(bmpInfo)
		delete bmpInfo;
	return result;
}


void LibraryCaller::SetASM()
{
	dllName = L"ASMFilter.dll";
}

void LibraryCaller::SetCPP()
{
	dllName = L"CPPFilter.dll";
}

void LibraryCaller::SetThreads(int x)
{
	threads = x;
	if (threads <= 0) {
		threads = GetCPUThreads();
	}
}

