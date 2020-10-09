#include "pch.h"
#include <cstdint>
#include <thread>
using namespace std;

const int mask[] = { 1,0,-1,1,1,-1,1,0,-1 };

struct ProcessedUnit {
	uint8_t*& pixels;
	uint8_t*& newPixels; 
	int rowOffset;
	int w;
};

void rowFilter(ProcessedUnit u) {
	constexpr int mask[] = { 1,0,-1,1,1,-1,1,0,-1 };
	for (int j = 1; j < u.w - 1; j++) {
		float sb = u.pixels[3 * (u.rowOffset - u.w + j - 1)] * mask[0];
		sb += u.pixels[3 * (u.rowOffset - u.w + j)] * mask[1];
		sb += u.pixels[3 * (u.rowOffset - u.w + j + 1)] * mask[2];
		sb += u.pixels[3 * (u.rowOffset + j - 1)] * mask[3];
		sb += u.pixels[3 * (u.rowOffset + j)] * mask[4];
		sb += u.pixels[3 * (u.rowOffset + j + 1)] * mask[5];
		sb += u.pixels[3 * (u.rowOffset + u.w + j - 1)] * mask[6];
		sb += u.pixels[3 * (u.rowOffset + u.w + j)] * mask[7];
		sb += u.pixels[3 * (u.rowOffset + u.w + j + 1)] * mask[8];

		float sg = u.pixels[3 * (u.rowOffset - u.w + j - 1) + 1] * mask[0];
		sg += u.pixels[3 * (u.rowOffset - u.w + j) + 1] * mask[1];
		sg += u.pixels[3 * (u.rowOffset - u.w + j + 1) + 1] * mask[2];
		sg += u.pixels[3 * (u.rowOffset + j - 1) + 1] * mask[3];
		sg += u.pixels[3 * (u.rowOffset + j) + 1] * mask[4];
		sg += u.pixels[3 * (u.rowOffset + j + 1) + 1] * mask[5];
		sg += u.pixels[3 * (u.rowOffset + u.w + j - 1) + 1] * mask[6];
		sg += u.pixels[3 * (u.rowOffset + u.w + j) + 1] * mask[7];
		sg += u.pixels[3 * (u.rowOffset + u.w + j + 1) + 1] * mask[8];

		float sr = u.pixels[3 * (u.rowOffset - u.w + j - 1) + 2] * mask[0];
		sr += u.pixels[3 * (u.rowOffset - u.w + j) + 2] * mask[1];
		sr += u.pixels[3 * (u.rowOffset - u.w + j + 1) + 2] * mask[2];
		sr += u.pixels[3 * (u.rowOffset + j - 1) + 2] * mask[3];
		sr += u.pixels[3 * (u.rowOffset + j) + 2] * mask[4];
		sr += u.pixels[3 * (u.rowOffset + j + 1) + 2] * mask[5];
		sr += u.pixels[3 * (u.rowOffset + u.w + j - 1) + 2] * mask[6];
		sr += u.pixels[3 * (u.rowOffset + u.w + j) + 2] * mask[7];
		sr += u.pixels[3 * (u.rowOffset + u.w + j + 1) + 2] * mask[8];

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

		u.newPixels[3 * (u.rowOffset + j)] = sb;
		u.newPixels[3 * (u.rowOffset + j) + 1] = sg;
		u.newPixels[3 * (u.rowOffset + j) + 2] = sr;
	}

}

int filter(uint8_t*& pixels, uint8_t*& newPixels, int w, int h) {
	thread* thlast = nullptr;
	for (int i = 1 * w; i < (h-1) * w; i += w) {
		if (thlast) {
			thlast->detach();
			thlast = nullptr;
		}
		thread* th = new thread(rowFilter, ProcessedUnit{pixels, newPixels, i, w});
		thlast = th;
	}
	thlast->join();
	thlast = nullptr;
	return 0;
}

