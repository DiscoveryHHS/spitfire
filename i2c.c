#include "i2c.h"
#include "usart.h"

void initI2c() {
	// Enable pullups
	PORTD |= (1 << PIND1) | (1 << PIND0);

	TWBR = 72; /* set I2C bit rate (p.242): 16MHz / (16+2*TWBR*1) ~= 100kHz */
	TWCR |= (1 << TWEN); // Enable i2c
}

void i2cWaitForComplete() {
	while (!(TWCR & (1 << TWINT)))
		;
}

void i2cStartOperation() {
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	i2cWaitForComplete();
}

void i2cStopOperation() {
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
	while (TWCR & (1 << TWSTO))
		;
}

void i2cSend(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	i2cWaitForComplete();
}

uint8_t i2cReadAck(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	i2cWaitForComplete();
	return (TWDR);
}

uint8_t i2cReadNoAck(void) {
	TWCR = (1 << TWEN) | (1 << TWINT);
	i2cWaitForComplete();
	return (TWDR);
}

uint8_t i2cGetStatus() {
	uint8_t status;
	status = TWSR & 0xF8;
	return status;
}

void setRegister(uint8_t r, uint8_t device) {
	i2cStartOperation();
	i2cSend(device << 1);   //i2c address master write
	i2cSend(r);
	i2cStopOperation();
}

uint8_t readReg8(uint8_t device) {
	i2cStartOperation();
	i2cSend((device << 1) + 1); //i2c address master read
	uint8_t x = i2cReadNoAck();
	i2cStopOperation();
	return x;
}

void writeStatus(uint8_t status) {
	writeString("Status: ");
	writeInt(status);
	writeString("\n\r");
}

uint16_t read16bitReg(uint8_t device, uint8_t address) {
	i2cStartOperation();

	i2cSend(device << 1);   //i2c address master write
	i2cSend(address);

	i2cStopOperation();
	i2cStartOperation();

	writeStatus(i2cGetStatus());

	i2cSend((device << 1) + 1); //i2c address master read
	writeStatus(i2cGetStatus());

	uint8_t x = i2cReadAck();
	writeStatus(i2cGetStatus());
	uint8_t y = i2cReadNoAck();
	writeStatus(i2cGetStatus());

	i2cStopOperation();

	writeString("X: ");
	writeInt(x);
	writeString("\n\rY: ");
	writeInt(y);
	writeString("\n\r");
	return ((y * 256) + x);
}

void writeReg(uint8_t address, uint8_t device, uint8_t data) {
	i2cStartOperation();
	i2cSend(device << 1);
	i2cSend(address);
	i2cSend(data);
	i2cStopOperation();
}
