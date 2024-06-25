#ifndef _RGBMATRIX_h
#define _RGBMATRIX_h

#include <Arduino.h>
#include <SPI.h>

#include "Pixel.h"

class RGBMatrix {
public:
	RGBMatrix();
	~RGBMatrix();

	void initSPI();
	void setPixel(byte x, byte y, byte r, byte g, byte b);
	void tick();
	void tickOne();
	uint32_t d;

private:
	const int latchPin = 5;

	byte tickOneCounter = 0;

	byte tickCounter = 0;
	byte cycleCounter = 0;
	const byte CYCLES_PER_PWM_CYCLE = 2;

	Pixel pixels[8][8];

	const byte colOrder[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	//const byte colOrder[8] = {0, 4, 1, 5, 2, 6, 3, 7};
	byte colMasks[8];
};

#endif