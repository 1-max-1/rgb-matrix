#include "PatternManager.h"

#define PATTERN_YIELD(duration)                 \
static unsigned long timeOfLastUpdate = 0;      \
unsigned long now = millis();                   \
if (now - timeOfLastUpdate < duration) return;  \
timeOfLastUpdate = now;                         \

#define isEven(X) ((X) % 2 == 0)

PatternManager::PatternManager(RGBMatrix* matrix) {
	this->matrix = matrix;
}

void PatternManager::fullHueFade() {
	static float hue = 0.f;
	for (int i = 0; i < 8; i++) {
		for (int row = 0; row < 8; row++) {
			matrix->setPixelHSL(i, row, hue, 1.0f, 0.5f);
		}
	}

	hue += 0.0001;
	if (hue >= 1.f)
		hue = 0.f;
}

void PatternManager::diagonalRainbowFade() {
	PATTERN_YIELD(1000)

	RGBMatrix* m = matrix;
	auto drawDiagonal = [m](float hue, int x, int y) {
		while (x < 8 && x >= 0 && y < 8 && y >= 0) {
			m->setPixelHSL(x, y, hue, 1.f, 0.5);
			x += 1; y += 1;
		}
	};

	static int start = 0;
	float hue = (float)start / 15.f;
	for (int row = 7; row >= 0; row--) {
		drawDiagonal(fmod(hue, 1), 0, row);
		hue += 1.f/15.f;
	}
	for (int col = 1; col < 8; col++) {
		drawDiagonal(fmod(hue, 1), col, 0);
		hue += 1.f/15.f;
	}

	start = (start + 1) % 15;
}

void PatternManager::tiles() {
	PATTERN_YIELD(3000)

	for (int x = 0; x <= 6; x += 2) {
		for (int y = 0; y <= 6; y += 2) {
			byte r = rand() > RAND_MAX / 2 ? 31 : 0;
			byte g = rand() > RAND_MAX / 2 ? 31 : 0;
			byte b = rand() > RAND_MAX / 2 ? 31 : 0;
			if (r == 0 && g == 0 && b == 0) {
				r = 31;
				g = 20;
			}

			matrix->setPixel(x, y, r, g, b);
			matrix->setPixel(x + 1, y, r, g, b);
			matrix->setPixel(x, y + 1, r, g, b);
			matrix->setPixel(x + 1, y + 1, r, g, b);
		}
	}
}

void PatternManager::concentricSquares() {
	PATTERN_YIELD(400);

	static byte offset = 0;
	const byte colors[4][3] = {{0, 20, 31}, {31, 10, 10}, {26, 26, 31}, {31, 31, 9}};

	matrix->clearPixels();
	for (int x = offset; x < (8 - offset); x++) {
		matrix->setPixel(x, offset, colors[offset][0], colors[offset][1], colors[offset][2]);
		matrix->setPixel(x, 7 - offset, colors[offset][0], colors[offset][1], colors[offset][2]);
	}
	for (int y = offset + 1; y < (8 - offset) - 1; y++) {
		matrix->setPixel(offset, y, colors[offset][0], colors[offset][1], colors[offset][2]);
		matrix->setPixel(7 - offset, y, colors[offset][0], colors[offset][1], colors[offset][2]);
	}

	offset = (offset + 1) % 4;
}

void PatternManager::spectrumAnalyzer() {
	PATTERN_YIELD(400)

	const byte colors[9][3] = {{0, 0, 0}, {31, 0, 0}, {31, 5, 0}, {31, 10, 0}, {31, 15, 0}, {31, 20, 0}, {31, 25, 0}, {31, 31, 0}, {31, 31, 13}};

	matrix->clearPixels();
	for (int i = 0; i < 8; i++) {
		byte level = rand() % 9;
		Serial.println(level);
		for (int y = 0; y < level; y++) {
			matrix->setPixel(i, 7 - y, colors[level][0], colors[level][1], colors[level][2]);
		}
	}	
}

void PatternManager::checker() {
	PATTERN_YIELD(4000);

	float hue1 = (float)rand() / (float)RAND_MAX;
	float hue2 = (float)rand() / (float)RAND_MAX;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			float hue = (isEven(row) == isEven(col)) ? hue1 : hue2;
			matrix->setPixelHSL(col, row, hue, 1.f, 0.5);
		}
	}
}