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
	LibraryCaller();
public:
	std::wstring inputName;
	std::wstring outputName;

	static LibraryCaller& instance();

	int ProcessImage(uint8_t* pixels, uint8_t* newPixels, int w, int h);
	int Run();

	void SetASM();
	void SetCPP();
	void SetThreads(int x);

};