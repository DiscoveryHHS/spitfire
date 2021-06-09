#include "gyro.h"

void readGyroX() {
	setRegister(0x28, GYRO_ADDR); // GYRO_XOUT_H
	uint16_t data = readReg16(GYRO_ADDR);
	
	writeString("X Gyro: ");
	writeInt(data);
	writeString("\n\r");
}

