#pragma once
#include <string>
#include <Windows.h>

typedef int(_fastcall* FilterProc)(uint8_t* pixels, uint8_t* newPixels, int rowOffset, int w);

enum ParseCode{Passed, ValueMiss, Exit};

class LibraryCaller {
	std::string inputFile;
	std::string outputFile;
	std::wstring dllName;
	unsigned int threads;
	HINSTANCE dllHandle;
	FilterProc filter;

	void LoadFilter();
	void UnloadFilter();
	unsigned int GetCPUThreads();
public:
	LibraryCaller();

	void ProcessImage(uint8_t* pixels, uint8_t* newPixels, int w, int h);
	void Run();
	ParseCode ParseArgs(const std::string& args);

	void SetThreads(int newThreads);
	void SetASM();
	void SetCPP();
};