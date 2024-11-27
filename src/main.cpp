#include <Arduino.h>
#include <RGBMatrix.h>

// Number of ticks of duration of the the timer that updates the matrix
// See https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/
const uint64_t MATRIX_TIMER_TICKS = 120;

RGBMatrix matrix;

void printClockVals() {
	Serial.print("CPU Freq = ");
	Serial.print(getCpuFrequencyMhz());
	Serial.println(" MHz");
	Serial.print("XTAL Freq = ");
	Serial.print(getXtalFrequencyMhz());
	Serial.println(" MHz");
	Serial.print("APB Freq = ");
	Serial.print(getApbFrequency());
	Serial.println(" Hz");
}

void setupColorTest();
void loopColorTest();
void setupTester();
void loopTester();
void setupSER();
void loopSER();

void IRAM_ATTR doMatrixTick() {
	matrix.tick();
}

void setup() {
	Serial.begin(115200);
	//printClockVals();
	//setupColorTest();
	//setupSER();

	matrix.initSPI();
	matrix.setAntiGhost(true);

	/*for (int i = 0; i < 8; i++) {
		matrix.setPixel(i, 0, 2, 0, 0);
		matrix.setPixel(i, 1, 0, 2, 0);
		matrix.setPixel(i, 2, 0, 0, 2);
		matrix.setPixel(i, 3, 2, 2, 0);
		matrix.setPixel(i, 4, 0, 2, 2);
		matrix.setPixel(i, 5, 2, 0, 2);
		matrix.setPixel(i, 6, 2, 2, 2);
		matrix.setPixel(i, 7, 2, 0, 1);
	}*/

	/*for (int i = 0; i < 8; i++) {
		for (int x = 0; x < 8; x++) {
			matrix.setPixel(x, i, i, 0, 0);
		}
	}*/

	matrix.setPixel(3, 1, 8, 0, 0);
	matrix.setPixel(3, 2, 8, 0, 0);
	matrix.setPixel(4, 5, 8, 0, 0);
	matrix.setPixel(4, 6, 8, 0, 0);

	hw_timer_t* Timer0_Cfg = timerBegin(2, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &doMatrixTick, true);
    timerAlarmWrite(Timer0_Cfg, MATRIX_TIMER_TICKS, true);
    timerAlarmEnable(Timer0_Cfg);
}

void loop() {
	// TODO: add wiring diagram

	//loopSER();
}