#include "RGBMatrix.h"

RGBMatrix::RGBMatrix() {
	// Pre-compute the row masks for every pixel and store them, this will save computation time later on
	for (byte row = 0; row < 8; row++) {
		for (Pixel& pixel : pixels[row]) {
			pixel.init(row);
		}
	}

	// Pre-compute the column masks and store them, this will save computation time later on
	for (byte i = 0; i < 8; i++) {
		colMasks[i] = (1 << (7 - colOrder[i]));
	}
}

RGBMatrix::~RGBMatrix() {
	SPI.end();
}

void RGBMatrix::setAntiGhost(bool use) {
	antiGhost = use;
}

void RGBMatrix::initSPI() {
	SPI.begin();
	SPI.setBitOrder(SPI_LSBFIRST);
	SPI.setHwCs(true); // Hardware chip-select

	// Anything much higher than this doesn't seem to work, possibly due to cross-talk in the PCB (bad routing naauurghhhh)
	SPI.setFrequency(5000000);
}

float RGBMatrix::hueToRGB(float p, float q, float t) {
	if (t < 0)
		t += 1;
	if (t > 1)
		t -= 1;
	if (t < 1./6)
		return p + (q - p) * 6 * t;
	if (t < 1./2)
		return q;
	if (t < 2./3)
		return p + (q - p) * (2./3 - t) * 6;
	return p;
}

void RGBMatrix::setPixelHSL(byte x, byte y, float h, float s, float l) {
	/* Modified from https://gist.github.com/ciembor/1494530 */

	if(0 == s) {
		byte val = l * 31; // achromatic
		setPixel(x, y, val, val, val);
	}
	else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		byte r = hueToRGB(p, q, h + 1./3) * 31;
		byte g = hueToRGB(p, q, h) * 31;
		byte b = hueToRGB(p, q, h - 1./3) * 31;
		setPixel(x, y, r, g, b);
	}
}

void RGBMatrix::setPixel(byte x, byte y, byte r, byte g, byte b) {
	pixels[y][x].setColor(gammaCorrections[r], gammaCorrections[g], gammaCorrections[b]);
}

void RGBMatrix::tick() {
	// One tick is one column
	uint32_t data = colMasks[tickCounter];
	byte x = colOrder[tickCounter];

	// The data consists of 4 bytes: RED, BLUE, GREEN, COLUMNS

	for (byte y = 0; y < 8; y++) {
		Pixel& pixel = pixels[y][x];
		byte pixelMask = pixel.getRowMask();
		if (cycleCounter < pixel.getR())
			data |= (pixelMask << 24);
		if (cycleCounter < pixel.getG())
			data |= (pixelMask << 8);
		if (cycleCounter < pixel.getB())
			data |= (pixelMask << 16);
	}
	data = ~data; // The physical circuit is wired with 1 for off and 0 for on, so we need to flip the bits

	if (antiGhost) {
		SPI.write32(0xFFFFFFFF);
	}
	SPI.write32(data);

	// Once we've gone through every column we can reset back to 0
	tickCounter++;
	if (tickCounter == 8) {
		tickCounter = 0;
		cycleCounter++;
	}

	if (cycleCounter == CYCLES_PER_PWM_CYCLE)
		cycleCounter = 0;
}