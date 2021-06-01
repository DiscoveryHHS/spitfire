#include "proximitysensors.h"

const uint8_t sensorChannels[] = {5, 8, 1};
const uint8_t sensorCount = 3;

void writeAllProximityValues() {
	uint8_t i;

	initADC(0);

	writeString("PR:");

	for (i = 0; i < sensorCount; i++) {
		setADCChannel(sensorChannels[i]);
		writeInt(readADCValue());
		writeChar(',');
	}

	writeChar('\n');
	writeChar('\r');

	stopADC();
}
