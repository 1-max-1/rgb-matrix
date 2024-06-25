#include "RGBMatrix.h"

RGBMatrix::RGBMatrix() {
	for (byte row = 0; row < 8; row++) {
		for (Pixel& pixel : pixels[row]) {
			pixel.init(row);
		}
	}

	for (byte i = 0; i < 8; i++) {
		colMasks[i] = (1 << (7 - colOrder[i]));
	}
}

RGBMatrix::~RGBMatrix() {
	SPI.end();
}

void RGBMatrix::initSPI() {
	pinMode(latchPin, OUTPUT);
	digitalWrite(latchPin, HIGH);

	SPI.begin();
	SPI.setBitOrder(SPI_LSBFIRST);
	SPI.setFrequency(5000000); // Not sure if it has to be a divisor of the APB frequency
	// TODO: play with SPI.setHwCs
}

void RGBMatrix::setPixel(byte x, byte y, byte r, byte g, byte b) {
	pixels[y][x].setColor(r, g, b);
}

void RGBMatrix::tick() {
	/*uint32_t dataR = 0xFF000000;
	uint32_t dataG = 0x0000FF00;
	uint32_t dataB = 0x00FF0000;

	byte x = colOrder[tickCounter];

	for (byte y = 0; y < 8; y++) {
		Pixel& pixel = pixels[y][x];
		byte pixelMask = pixel.getRowMask();
		if (cycleCounter < pixel.getR())
			dataR &= (pixelMask << 24);
		if (cycleCounter < pixel.getG())
			dataG &= (pixelMask << 8);
		if (cycleCounter < pixel.getB())
			dataB &= (pixelMask << 16);
	}
	uint32_t data = dataR | dataG | dataB | colMasks[tickCounter];*/

	uint32_t data = colMasks[tickCounter];
	byte x = colOrder[tickCounter];

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
	data = ~data; // Physical circuit is wired with 1 for off and 0 for on, so we need to flip the bits
	d = data;
	// TODO: fix ghosting, set rows to off first

	digitalWrite(latchPin, LOW);
	SPI.write32(data);
	digitalWrite(latchPin, HIGH);

	tickCounter++;
	if (tickCounter == 8) {
		tickCounter = 0;
		cycleCounter++;
	}

	if (cycleCounter == CYCLES_PER_PWM_CYCLE)
		cycleCounter = 0;
}

void RGBMatrix::tickOne() {
	uint32_t dataR = 0xFF000000;
	uint32_t dataG = 0x0000FF00;
	uint32_t dataB = 0x00FF0000;

	byte x = colOrder[tickCounter];
	Pixel& pixel = pixels[tickOneCounter][x];
	byte pixelMask = pixel.getRowMask();

	if (cycleCounter < pixel.getR())
		dataR &= (pixelMask << 24);
	if (cycleCounter < pixel.getG())
		dataG &= (pixelMask << 8);
	if (cycleCounter < pixel.getB())
		dataB &= (pixelMask << 16);
	uint32_t data = dataR | dataG | dataB | colMasks[tickCounter];

	// TODO: fix ghosting, set rows to off first

	digitalWrite(latchPin, LOW);
	SPI.write32(data);
	digitalWrite(latchPin, HIGH);

	tickOneCounter++;
	if (tickOneCounter == 3) {
		tickOneCounter = 0;
		tickCounter++;
	}

	if (tickCounter == 8) {
		tickCounter = 0;
		cycleCounter++;
	}

	if (cycleCounter == CYCLES_PER_PWM_CYCLE)
		cycleCounter = 0;
}