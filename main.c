#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "motors.h"
#include "usart.h"
#include "encoders.h"
#include "io.h"
#include "eeprom.h"
#include "timer.h"
#include "proximitysensors.h"

// Left encoder
ISR(PCINT0_vect) {
	leftEncoderInterruptHandler();
}

// Right encoder
ISR(INT6_vect) {
	rightEncoderInterruptHandler();
}

ISR(TIMER0_COMPA_vect) {
	static uint8_t counter;

	if (counter ++ > 180) {
		// ~ 3 sec interrupt

		writeAllProximityValues();

		counter = 0;
	}
}

ISR(USART1_RX_vect) {
	char send = UDR1;

	static char usartBuffer[34];
	static uint8_t usartBufferLength = 0;

	usartBuffer[usartBufferLength] = send;
	usartBufferLength++;

	// When return is send, we parse the buffer for a command with parameters.
	if (send == '\n' || send == 13) {
		char command[3];
		char params[4][8];

		usartSplitToCommand(usartBuffer, usartBufferLength, command, params);
		usartBufferLength = 0;

		// SM : SET_MOTOR command.
		if (command[0] == 'S' && command[1] == 'M') {
			setLeftSpeed(atoi(params[0]), atoi(params[2]));
			setRightSpeed(atoi(params[1]), atoi(params[3]));
		}
		// GE : GET_ENCODERS query.
		else if (command[0] == 'G' && command[1] == 'E') {
			writeString("EC:");
			writeInt(getLeftEncoderTicks());
			writeChar(',');
			writeInt(getRightEncoderTicks());
			writeChar('\n');
		}
	}
}

int main() {
	initEncoders();
	initUsart();
	initButtons();
	startMotors();
	initTimer0();

	calibrateMotors();

	while (1)
		;

	return (0);
}
