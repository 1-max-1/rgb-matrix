#ifndef RGB_MATRIX_PIXEL_h
#define RGB_MATRIX_PIXEL_h

typedef unsigned char byte;

struct Pixel {
public:
	// Builds row mask for this pixel. Must call this once before using this pixel structure.
	void init(byte row);
	void setColor(byte r, byte g, byte b);

	byte getR();
	byte getG();
	byte getB();

	// Returns the row mask for this pixel.
	// All bit positions are 1 apart from the the position corresponding to the row for this pixel.
	// The MSB is row 0, the LSB is row 7. e.g. 0b11011111 means this pixel is in row 2. The row should be set with init().
	byte getRowMask();

private:
	byte r = 0;
	byte g = 0;
	byte b = 0;

	byte rowMask = 0;
};

#endif