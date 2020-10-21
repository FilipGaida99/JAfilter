#pragma once
#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <string>

//Funkcja wczytuj¹ca bitmapê na podstawie nazwy. 
//3 ostatnie argumenty s¹ argumentami wyjœciowymi i powinny byæ zainicjowane jako nullptr.
bool ReadBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);
//Funkcja zapisuj¹ca bitmapê na podstawie nazwy, tablicy pixeli oraz struktur  z informacjami o bitmapie. 
bool SaveBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo);