#pragma once
#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <string>

bool ReadBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);
bool SaveBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);