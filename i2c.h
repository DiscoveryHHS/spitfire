#include <avr/io.h>

void initI2c();
void i2cWaitForComplete();
void i2cStartOperation();
void i2cStopOperation();
void i2cSend(uint8_t data);
uint8_t i2cReadAck(void);
uint8_t i2cReadNoAck(void);
void setRegister(uint8_t r, uint8_t device);
uint8_t readReg8(uint8_t device);
uint16_t readReg16(uint8_t device);
void writeReg(uint8_t address, uint8_t device, uint8_t data);
