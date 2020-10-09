#include <iostream>
#include <Windows.h>
#include "FileInterface.h"

typedef int(_fastcall* FilterProc)(uint8_t*& pixels, uint8_t*& newPixels, int w, int h);

int main()
{

    HINSTANCE dllHandle = NULL;
    dllHandle = LoadLibrary(L"CPPFilter.dll");
    FilterProc filter = (FilterProc)GetProcAddress(dllHandle, "filter");
    uint8_t* pixels = nullptr;
    BITMAPFILEHEADER* bmpHeader = nullptr;
    BITMAPINFOHEADER* bmpInfo = nullptr;
    ReadBMP("test.bmp", pixels, bmpHeader, bmpInfo);
    uint8_t* newPixels = new uint8_t[bmpInfo->biSizeImage];
    int i = filter(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
    SaveBMP("testsave.bmp", newPixels, bmpHeader, bmpInfo);
    std::cout << "Hello World!\n"<< i;

}
