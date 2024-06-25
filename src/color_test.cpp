#include <Arduino.h>
#include <SPI.h>

const int LATCH_PIN = 5;

void writeState(byte x, bool red, bool green, bool blue) {
	byte reds = red ? 0b00011111 : 0xFF;
	byte greens = green ? 0b00011111 : 0xFF;
	byte blues = blue ? 0b00011111 : 0xFF;
	uint32_t data = (reds << 24) | (blues << 16) | (greens << 8) | (1 << (7 - x));
	digitalWrite(LATCH_PIN, LOW);
	SPI.write32(data);
	digitalWrite(LATCH_PIN, HIGH);
}

void setupColorTest() {
	pinMode(LATCH_PIN, OUTPUT);
	digitalWrite(LATCH_PIN, HIGH);
	SPI.begin();
	SPI.setFrequency(5000000);
	SPI.setBitOrder(SPI_LSBFIRST);
}

void loopColorTest() {
	writeState(1, true, false, false);
	delay(4000);
	writeState(2, false, true, false);
	delay(4000);
	writeState(3, false, false, true);
	delay(4000);
	writeState(4, true, true, false);
	delay(4000);
	writeState(5, true, false, true);
	delay(4000);
	writeState(6, false, true, true);
	delay(4000);
	writeState(7, true, true, true);
	delay(4000);
}