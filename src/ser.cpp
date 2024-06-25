#include <Arduino.h>
#include <SPI.h>

const int LATCH_PIN = 5;

void writeState(uint32_t data) {
	digitalWrite(LATCH_PIN, LOW);
	SPI.write32(data);
	digitalWrite(LATCH_PIN, HIGH);
}

void setupSER() {
	pinMode(LATCH_PIN, OUTPUT);
	digitalWrite(LATCH_PIN, HIGH);
	SPI.begin();
	SPI.setBitOrder(SPI_LSBFIRST);
	writeState((0b00000000 << 24) | (0b00000000 << 16) | (0b00000000 << 8) | 0b00000000);
}

void loopSER() {
	if (Serial.available() >= 4) {
		byte buf[4];
		Serial.readBytes(buf, 4);
		uint32_t data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
		writeState(data);
	}
}