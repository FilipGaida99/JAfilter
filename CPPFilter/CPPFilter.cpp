#include "pch.h"
#include <cstdint>
using namespace std;

const int mask[] = { 1,0,-1,1,0,-1,1,0,-1 };

void filter(uint8_t* pixels, uint8_t* newPixels, int proccesedSize, int w) {
	constexpr int mask[] = { 1,0,-1,1,1,-1,1,0,-1 };
	for (int j = 0; j < proccesedSize; j+=3) {
		float sb = pixels[- w + j - 3] * mask[0];
		sb += pixels[- w + j] * mask[1];
		sb += pixels[- w + j + 3] * mask[2];
		sb += pixels[+ j - 3] * mask[3];
		sb += pixels[+ j] * mask[4];
		sb += pixels[+ j + 3] * mask[5];
		sb += pixels[+ w + j - 3] * mask[6];
		sb += pixels[+ w + j] * mask[7];
		sb += pixels[+ w + j + 3] * mask[8];

		float sg = pixels[- w + j - 3 + 1] * mask[0];
		sg += pixels[- w + j+1] * mask[1];
		sg += pixels[- w + j + 3+1] * mask[2];
		sg += pixels[+ j - 3+1] * mask[3];
		sg += pixels[+ j+1] * mask[4];
		sg += pixels[+ j + 3+1] * mask[5];
		sg += pixels[+ w + j - 3+1] * mask[6];
		sg += pixels[+ w + j+1] * mask[7];
		sg += pixels[+ w + j + 3+1] * mask[8];

		float sr = pixels[- w + j - 3+2] * mask[0];
		sr += pixels[- w + j+2] * mask[1];
		sr += pixels[- w + j + 3+2] * mask[2];
		sr += pixels[+ j - 3+2] * mask[3];
		sr += pixels[+ j+2] * mask[4];
		sr += pixels[+ j + 3+2] * mask[5];
		sr += pixels[+ w + j - 3+2] * mask[6];
		sr += pixels[+ w + j+2] * mask[7];
		sr += pixels[+ w + j + 3+2] * mask[8];

		if (sr > 255) {
			sr = 255;
		}
		if (sr < 0) {
			sr = 0;
		}
		if (sb > 255) {
			sb = 255;
		}
		if (sb < 0) {
			sb = 0;
		}
		if (sg > 255) {
			sg = 255;
		}
		if (sg < 0) {
			sg = 0;
		}

		newPixels[+ j] = sb;
		newPixels[+ j + 1] = sg;
		newPixels[+ j + 2] = sr;
	}

}