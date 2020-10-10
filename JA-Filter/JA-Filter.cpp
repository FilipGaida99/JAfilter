#include <iostream>
#include "FileInterface.h"
#include "LibraryCaller.h"

int main(int argc, char* arg[])
{
    LibraryCaller library;
    library.ParseArgs(argc, arg);

    uint8_t* pixels = nullptr;
    BITMAPFILEHEADER* bmpHeader = nullptr;
    BITMAPINFOHEADER* bmpInfo = nullptr;
    ReadBMP("test.bmp", pixels, bmpHeader, bmpInfo);
    uint8_t* newPixels = new uint8_t[bmpInfo->biSizeImage];
    int i = library.ProcessImage(pixels, newPixels, bmpInfo->biWidth, bmpInfo->biHeight);
    SaveBMP("testsave.bmp", newPixels, bmpHeader, bmpInfo);

    delete[] pixels;
    delete[] newPixels;
    delete bmpHeader;
    delete bmpInfo;
}
