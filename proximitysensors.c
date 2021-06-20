#include "irleds.h"
#include "adc.h"
#include "usart.h"

#include "proximitysensors.h"

const uint8_t sensorChannels[] = { 5, 1, 8 }; //adc channels voor prox sensors
const uint8_t sensorCount = 3;

volatile uint16_t sensorValues[3];

void updateProximityValues() {
	setIRBrightness(400);

	// Go through all sensors
	uint8_t i;
	for (i = 0; i < sensorCount; i++) {
		if (i == 0)
			setIRLedsDirection(twoLeftIRLeds);
		else
			setIRLedsDirection(twoRightIRLeds);

		setADCChannel(sensorChannels[i]);
		sensorValues[i] = readADCValue();
	}
}

void writeAllProximityValues() {
	uint8_t i;

	writeString("PR:");
	for (i = 0; i < sensorCount; i++) {
		writeInt(sensorValues[i]);
		writeChar(',');
	}
	writeChar('\n');
}

void initIRLeds() {
	startIRLeds();
}
