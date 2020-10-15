#include "pch.h"
#include <cstdint>
using namespace std;


void filter(uint8_t* pixels, uint8_t* newPixels, int dataSize, int w) {
	constexpr int mask[] = { 1, 0, -1,
							 1, 1, -1,
							 1, 0, -1 };
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

		if (s > 255) {
			s = 255;
		}
		if (s < 0) {
			s = 0;
		}

		newPixels[j] = s;
	}

}