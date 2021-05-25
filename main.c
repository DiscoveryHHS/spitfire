#include <avr/io.h>
#include <avr/interrupt.h>

#include "motors.h"
#include "usart.h"
#include "encoders.h"
#include "io.h"
#include "eeprom.h"

// Left encoder
ISR(PCINT0_vect) {
	leftEncoderInterruptHandler();
}

// Right encoder
ISR(INT6_vect) {
	rightEncoderInterruptHandler();
}

// TODO: needs to be refactored
//ISR(USART1_RX_vect) {
//	char send = UDR1;
//
//	static char usartBuffer[32];
//	static uint8_t usartBufferLength;
//
//	usartBuffer[usartBufferLength] = send;
//	usartBufferLength++;
//
//	if (send == '\n' || send == 15) {
//		//void executeQuery(usartBuffer, usartBufferLength);
//	}
//}

int main() {
	initEncoders();
	initUsart();
	initButtons();
	startMotors();

	calibrateMotors();
	return(0);
}
