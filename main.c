/*
 * Zumo 32u4.c
 *
 * Created: 2021-05
 * Authors : Wouter de Bruijn
 * 			 Stan Albada Jelgersma
 */

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "motors.h"
#include "usart.h"
#include "encoders.h"
#include "io.h"
#include "eeprom.h"
#include "buzzer.h"
#include "adc.h"
#include "timer.h"
#include "proximitysensors.h"
#include "gyro.h"

//pin change interrupt, handles all pin change interrupts (included buttons A and C)
ISR(PCINT0_vect) {
	leftEncoderInterruptHandler();
	buttonsInterruptHandler();
}

// Right encoder
ISR(INT6_vect) {
	rightEncoderInterruptHandler();
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

//gaat ongeveer 60 keer per seconde af
ISR(TIMER0_COMPA_vect) {
	static uint8_t timer0Counter = 0;

	timer0Counter++;

	if (timer0Counter >= 60) {
		//start cycle adc conversies
		startADCProximityCycle();

		timer0Counter = 0;
	}
}

ISR(ADC_vect) {
	proxSensADCInterruptHandler();
}

int main() {
	initEncoders();
	initUsart();
	initButtons();
	startMotors();
	startBuzzerTimer();
	playBuzzerStartupSound();
	stopBuzzerTimer();
	calibrateMotors();
	initIRLeds();
	initTimer0();
	initADC(1);
	
	initI2c();
	
	// Enable chip
	writeReg(0x6B, GYRO_ADDR, 0);

	writeReg(0x19, GYRO_ADDR, 7); // 8Khz / (1 + 7) = 1Khz

	// Set full range for both degrees and acceleration
	writeReg(0x1B, GYRO_ADDR, 0);
	writeReg(0x1C, GYRO_ADDR, 0);

	setRegister(0x75, GYRO_ADDR); // WHO_AM_I
	uint8_t gyroResponse = readReg(GYRO_ADDR);
	writeInt(gyroResponse);
	writeString(" == 104 [Gyro send first]\n\r");

	while (1) {
		readGyroX();
		readAccelX();

		_delay_ms(1000);
	};
	
	return (0);
}

