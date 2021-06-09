#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "motors.h"
#include "eeprom.h"

void resetEncoderTicks();
void initEncoders();
void stopEncoders();
void leftEncoderInterruptHandler();
void rightEncoderInterruptHandler();
void calibrateMotors();
uint64_t getLeftEncoderTicks();
uint64_t getRightEncoderTicks();
void getCalibrationDataFromEEPROM();
double getLeftMotorRatio();
double getRightMotorRatio();
