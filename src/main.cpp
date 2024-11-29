#include <Arduino.h>
#include <RGBMatrix.h>

// Number of ticks of duration of the the timer that updates the matrix
// See https://deepbluembedded.com/esp32-timers-timer-interrupt-tutorial-arduino-ide/
const uint64_t MATRIX_TIMER_TICKS = 45;

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

	for (int i = 0; i < 8; i++) {
		matrix.setPixel(i, 0, 31, 0, 0);
		matrix.setPixel(i, 1, 0, 31, 0);
		matrix.setPixel(i, 2, 0, 0, 31);
		matrix.setPixel(i, 3, 31, 31, 0);
		matrix.setPixel(i, 4, 0, 31, 31);
		matrix.setPixel(i, 5, 31, 0, 31);
		matrix.setPixel(i, 6, 31, 31, 31);
	}

	hw_timer_t* Timer0_Cfg = timerBegin(2, 80, true);
    timerAttachInterrupt(Timer0_Cfg, &doMatrixTick, true);
    timerAlarmWrite(Timer0_Cfg, MATRIX_TIMER_TICKS, true);
    timerAlarmEnable(Timer0_Cfg);
}

void loop() {
	// TODO: add wiring diagram
}