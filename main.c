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
//	initEncoders();
	initUsart();
//	initButtons();
//	startMotors();
//	startBuzzerTimer();
//	playBuzzerStartupSound();
//	stopBuzzerTimer();
//	calibrateMotors();
//	initIRLeds();
//	initTimer0();
//	initADC(1);
	
	initI2c();
	
	// Enable Gyro
	writeReg(0x20, GYRO_ADDR, 0b01111111); // DR = 01 (189.4 Hz ODR); BW = 11 (70 Hz bandwidth); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
	writeReg(0x23, GYRO_ADDR, 0x20); // FS = 10 (+/- 2000 dps full scale)

	// Enable Magnetometer
//	writeReg(0x24, MEGN_ADDR, 0x64); // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
//	writeReg(0x25, MEGN_ADDR, 0x20); // MFS = 01 (+/- 4 gauss full scale)
//	writeReg(0x26, MEGN_ADDR, 0x00); // MD = 00 (continuous-conversion mode)

	writeString("Good till now \n\r");

	while (1) {
		readGyroX();

//		setRegister(0x08, MEGN_ADDR); // GYRO_XOUT_H
//		uint16_t data = readReg16(MEGN_ADDR);
//
//		writeString("MEGN: ");
//		writeInt(data);
//		writeString("\n\r");

		_delay_ms(1000);
	};
	
	return (0);
}

