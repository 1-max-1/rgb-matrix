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

void RGBMatrix::initSPI() {
	SPI.begin();
	SPI.setBitOrder(SPI_LSBFIRST);
	SPI.setHwCs(true);
	
	// Not sure if the SPI frequency has to be a divisor of the APB frequency.
	// Also anything much higher than this doesn't seem to work, possibly due to excessive cross-talk in the PCB (bad routing oops)
	SPI.setFrequency(5000000);
}

void RGBMatrix::setPixel(byte x, byte y, byte r, byte g, byte b) {
	pixels[y][x].setColor(r, g, b);
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
	// TODO: fix ghosting, set rows to off first

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