#include <Arduino.h>
#include <RGBMatrix.h>

// Number of ticks of duration of the the timer that updates the matrix
// See https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/
// Ensure this is not lower than the time it takes to execute the interrupt. Each call to SPI.write() is ~15 us.
const uint64_t MATRIX_TIMER_TICKS = 45; // Microseconds

/**
 * The code uses the default setup for the SPI variable, so the matrix should be wired to the default VSPI pins.
 * MOSI - 23
 * CLK - 18
 * CS/SS - 5
 */

RGBMatrix matrix;

void IRAM_ATTR doMatrixTick() {
	matrix.tick();
}

void setup() {
	Serial.begin(115200);

	matrix.initSPI();
	matrix.setAntiGhost(true);

	/*for (int i = 0; i < 8; i++) {
		matrix.setPixel(i, 0, 31, 0, 0);
		matrix.setPixel(i, 1, 0, 31, 0);
		matrix.setPixel(i, 2, 0, 0, 31);
		matrix.setPixel(i, 3, 31, 31, 0);
		matrix.setPixel(i, 4, 0, 31, 31);
		matrix.setPixel(i, 5, 31, 0, 31);
		matrix.setPixel(i, 6, 31, 31, 31);
	}*/

	hw_timer_t* Timer0_Cfg = timerBegin(2, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &doMatrixTick, true);
    timerAlarmWrite(Timer0_Cfg, MATRIX_TIMER_TICKS, true);
    timerAlarmEnable(Timer0_Cfg);
}

float hue = 0.f;

void loop() {
	for (int i = 0; i < 8; i++) {
		for (int row = 0; row < 8; row++) {
			matrix.setPixelHSL(i, row, hue, 1.0f, 0.5f);
		}
	}

	hue += 0.0001;
	if (hue >= 1.f)
		hue = 0.f;
}