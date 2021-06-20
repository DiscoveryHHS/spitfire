#include "autodrive.h"
#include "motors.h"
#include "advProximity.h"

volatile uint8_t enabled = 0;
volatile uint8_t firstCycle = 1;

// Only runs first round after disable / on startup.
void init() {
	stopADC();
	advInitADC(0);
}

void cycle() {
	if (enabled == 1) {
		if (firstCycle == 1) {
			init();
			firstCycle = 0;
		}

		advStartADCProximityCycle();
		obstacleAvoider();
	}
}

void enable() {
	enabled = 1;
}

void disable() {
	firstCycle = 1;
	advStopADC();
	initADC(1);
	enabled = 0;
}

uint8_t getStatus() {
	return enabled;
}
