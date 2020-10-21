#pragma once
#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <string>

//Funkcja wczytuj�ca bitmap� na podstawie nazwy. 
//3 ostatnie argumenty s� argumentami wyj�ciowymi i powinny by� zainicjowane jako nullptr.
bool ReadBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);
//Funkcja zapisuj�ca bitmap� na podstawie nazwy, tablicy pixeli oraz struktur  z informacjami o bitmapie. 
bool SaveBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);