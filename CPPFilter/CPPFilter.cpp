//Temat: Filtrowanie uwypuklaj¹ce skierowane na zachód.
//Algorytm bêdzie filtrowa³ ka¿dy z kolorów pixeli zgodnie z mask¹
//1 0 -1
//1 1 -1
//1 0 -1
//Wykona³: Filip Gaida gr.3 semestr 5, 2020/2021

#include "pch.h"
#include <cstdint>
using namespace std;

//Funkcja filtruj¹ca
//pixels - tablica wejœciowa, newPixels-tablica wyjœciowa, 
//dataSize- rozmiar danych do przetworzenia. w- szerokoœæ jednego wiersza.
void filter(uint8_t* pixels, uint8_t* newPixels, int dataSize, int w) {
	constexpr int mask[] = { 1, 0, -1,
							 1, 1, -1,
							 1, 0, -1 }; //Maska z wagami filtrowania.
	for (int j = 0; j < dataSize; j+=1) {
		int s = pixels[- w + j - 3] * mask[0];
		s += pixels[- w + j] * mask[1];
		s += pixels[- w + j + 3] * mask[2];
		s += pixels[j - 3] * mask[3];
		s += pixels[j] * mask[4];
		s += pixels[j + 3] * mask[5];
		s += pixels[w + j - 3] * mask[6];
		s += pixels[w + j] * mask[7];
		s += pixels[w + j + 3] * mask[8];

		//Ograniczenie danych do zakresu.
		if (s > 255) {
			s = 255;
		}
		if (s < 0) {
			s = 0;
		}

		newPixels[j] = s;
	}

}