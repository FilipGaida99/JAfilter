#include "LibraryCaller.h"
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

LibraryCaller::LibraryCaller(): dllName(L"ASMFilter.dll"), threads(GetCPUThreads()), dllHandle(NULL), filter(nullptr)
{
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

bool LibraryCaller::ParseArgs(int argc, char* arg[])
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(arg[i], "-asm")) {
			dllName = L"ASMFilter.dll";
		}
		else if (!strcmp(arg[i], "-cpp")) {
			dllName = L"CPPFilter.dll";
		}
		else if (!strcmp(arg[i], "-thr")) {
			if (argc > (i + 1))
				threads = atoi(arg[++i]);
			else
				return false;
		}
	}
	return true;
}