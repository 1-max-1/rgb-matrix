#include <Arduino.h>
#include <SPI.h>

const int LATCH_PIN = 5;

void writeState(byte cols, byte reds, byte greens, byte blues) {
	uint32_t data = (reds << 24) | (blues << 16) | (greens << 8) | cols;
	digitalWrite(LATCH_PIN, LOW);
	SPI.write32(data);
	digitalWrite(LATCH_PIN, HIGH);
}

void setupTester() {
	pinMode(LATCH_PIN, OUTPUT);
	digitalWrite(LATCH_PIN, HIGH);
	SPI.begin();
	SPI.setBitOrder(SPI_LSBFIRST);
	//writeState(0b01000000, 0b11111111, 0b11111111, 0b11111111);
}

void loopTester() {
	for (int col = 0; col < 8; col++) {
		byte colData = 1UL << (7 - col);
		for (int row = 0; row < 8; row++) {
			byte colorData = ~(1UL << (7 - row));
			writeState(colData, colorData, 0xFF, 0xFF);
			delay(250);
			writeState(colData, 0xFF, colorData, 0xFF);
			delay(250);
			writeState(colData, 0xFF, 0xFF, colorData);
			delay(250);
		}
	}
}