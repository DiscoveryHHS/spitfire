#include "gyro.h"

void readGyroX() {
	setRegister(0x43, GYRO_ADDR); // GYRO_XOUT_H
	uint8_t x1 = readReg(GYRO_ADDR);
	
	setRegister(0x44, GYRO_ADDR); // GYRO_XOUT_L
	uint8_t x2 = readReg(GYRO_ADDR);
	
	
	uint16_t x = ((x1 * 256) + x2) / 131.0;
	
	writeString("X Gyro: ");
	writeInt(x);
	writeString("\n\r");
}

void readAccelX() {
	setRegister(0x3B, GYRO_ADDR); // ACCEL_XOUT_H
	uint8_t x1 = readReg(GYRO_ADDR);
	
	setRegister(0x3C, GYRO_ADDR); // ACCEL_XOUT_L
	uint8_t x2 = readReg(GYRO_ADDR);
	
	
	uint16_t x = ((x1 * 256) + x2) / 16384;
	
	writeString("X Accel: ");
	writeInt(x);
	writeString("\n\r");
}