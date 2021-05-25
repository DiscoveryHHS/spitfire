#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "motors.h"

void resetEncoderTicks();
void initEncoders();
void leftEncoderInterruptHandler();
void rightEncoderInterruptHandler();
void calibrateMotors();
uint64_t getLeftEncoderTicks();
uint64_t getRightEncoderTicks();
