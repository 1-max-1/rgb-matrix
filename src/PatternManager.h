#ifndef _PATTERN_MANAGER_h
#define _PATTERN_MANAGER_h

#include <RGBMatrix.h>

class PatternManager {
	public:
	PatternManager(RGBMatrix* matrix);

	// Fading hue rainbow, every pixel the same color
	void fullHueFade();

	// Hue rainbow, on the diagonals. The starting point shifts periodically.
	void diagonalRainbowFade();

	// 2x2 squares, chaotic colors
	void tiles();

	// 4 squares in eachother
	void concentricSquares();

	// Like n audio spectrum analyzer, but random and chaotic data
	void spectrumAnalyzer();

	// Checkerboard
	void checker();

	private:
	RGBMatrix* matrix;
};

#endif