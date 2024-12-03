#include <Arduino.h>
#include <RGBMatrix.h>
#include "PatternManager.h"

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
PatternManager patterns(&matrix);

void IRAM_ATTR doMatrixTick() {
	matrix.tick();
}

void setup() {
	Serial.begin(115200);

	matrix.initSPI();
	matrix.setAntiGhost(true);

	hw_timer_t* Timer0_Cfg = timerBegin(2, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &doMatrixTick, true);
    timerAlarmWrite(Timer0_Cfg, MATRIX_TIMER_TICKS, true);
    timerAlarmEnable(Timer0_Cfg);
}

void loop() {
	patterns.checker();
}