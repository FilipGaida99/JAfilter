#pragma once
#include <string>
#include <Windows.h>

typedef int(_fastcall* FilterProc)(uint8_t* pixels, uint8_t* newPixels, int rowOffset, int w);


class LibraryCaller {
	std::wstring dllName;
	unsigned int threads;
	HINSTANCE dllHandle;
	FilterProc filter;

	void LoadFilter();
	void UnloadFilter();
	unsigned int GetCPUThreads();
public:
	LibraryCaller();

	int ProcessImage(uint8_t* pixels, uint8_t* newPixels, int w, int h);
	bool ParseArgs(int argc, char* arg[]);

};