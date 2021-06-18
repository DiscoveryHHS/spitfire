#include "gyro.h"

void readGyroX() {
	uint16_t d = read16bitReg(GYRO_ADDR, 0x2A);
	writeInt(d);
	writeString("\n\r");
}

