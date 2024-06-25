#include "Pixel.h"

void Pixel::init(byte row) {
	rowMask = (1 << (7 - row));
}

void Pixel::setColor(byte r, byte g, byte b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

byte Pixel::getR() {
	return r;
}

byte Pixel::getG() {
	return g;
}

byte Pixel::getB() {
	return b;
}

byte Pixel::getRowMask() {
	return rowMask;
}