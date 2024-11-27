#ifndef _RGBMATRIX_h
#define _RGBMATRIX_h

#include <Arduino.h>
#include <SPI.h>

#include "Pixel.h"

class RGBMatrix {
public:
	// The initSPI() method should be called next to setup SPI for communication with the matrix.
	// When the destructor is called, the SPI bus will be stopped (with SPI.end() ).
	RGBMatrix();
	~RGBMatrix();

	// Setup SPI with correct settings to communicate with the matrix.
	void initSPI();

	// Sets the color of the given pixel. RGB values can range from 0 to 2
	void setPixel(byte x, byte y, byte r, byte g, byte b);

	// Performs the multiplexing cycle. Moves to the next column and updates the matrix. Call this method on a fixed interval.
	void tick();

	void setAntiGhost(bool use);

private:
	byte tickCounter = 0;  // Corresponds to one column/call of tick(). After 8 ticks, it resets to 0.
	byte cycleCounter = 0; // One full cycle of all 8 columns in a matrix. Equal to 8 ticks.
	const byte CYCLES_PER_PWM_CYCLE = 8; // PWM cycle is number of cycles that must be completed for a full brightness LED.

	Pixel pixels[8][8];

	const byte colOrder[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	// IDEA: try use irregular column order to reduce flickering
	//const byte colOrder[8] = {0, 4, 1, 5, 2, 6, 3, 7};

	// The mask for each column, where all bits are 0 except for a 1 in the position of the column.
	// Each mask maps to the corresponding element in colOrder.
	byte colMasks[8];

	bool antiGhost = false;
};

#endif