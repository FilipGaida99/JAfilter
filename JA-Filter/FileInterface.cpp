#include "FileInterface.h"

bool ReadBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo)
{

	uint8_t* datBuff[2] = { nullptr, nullptr }; // Bufor na informacje o bitmapie.

	FILE* file = fopen(filename, "rb");
	if (!file)
	{
		printf("Blad otwarcia pliku wejsciowego.\n");
		return false;
	}

	// Alokacja pamiêci na dane o bitmapie.
	datBuff[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
	datBuff[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

	fread(datBuff[0], sizeof(BITMAPFILEHEADER), 1, file);
	fread(datBuff[1], sizeof(BITMAPINFOHEADER), 1, file);
	bmpHeader = (BITMAPFILEHEADER*)datBuff[0];
	bmpInfo = (BITMAPINFOHEADER*)datBuff[1];

	// Sprawdzenie poprawnoœci formatu pliku.
	if (bmpHeader->bfType != 0x4D42)
	{
		printf("Plik nie jest bitmapa.\n");
		return false;
	}
	// Alokowanie pamiêci dla plikseli.
	pixels = new uint8_t[bmpInfo->biSizeImage];

	// Za³adowanie pikseli.
	fseek(file, bmpHeader->bfOffBits, 0);
	fread((char*)pixels, sizeof(uint8_t), bmpInfo->biSizeImage, file);
	fclose(file);
	return true;
}


bool SaveBMP(const char* filename, uint8_t*& pixels, BITMAPFILEHEADER*& bmpHeader, BITMAPINFOHEADER*& bmpInfo)
{
	//Create a new file for writing
	FILE* pFile = fopen(filename, "wb");
	if (pFile == NULL)
	{
		return false;
	}
	bmpHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmpHeader->bfSize = sizeof(BITMAPFILEHEADER);
	bmpInfo->biSize = sizeof(BITMAPINFOHEADER);
	fwrite(bmpHeader, bmpHeader->bfSize, 1, pFile);
	fwrite(bmpInfo, bmpInfo->biSize, 1, pFile);
	fwrite(pixels, 1, bmpInfo->biSizeImage, pFile);
	fclose(pFile);

	return true;
}