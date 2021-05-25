#define F_CPU	160000000

#include <avr/io.h>

#include "motors.h"
#include "usart.h"
#include "encoders.h"

ISR(PCINT0_vect) {
	leftEncoderInterruptHandler();
}

//rechter encoder
ISR(INT6_vect) {
	rightEncoderInterruptHandler();
}

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

//initializes all buttons on 32u4 board (a and c, b has no pin change interrupt)
void initButtons() {
	//ddr registers voor alle knoppen
	DDRB &= ~((1 << PORTB3) | (1 << PORTB0));
	DDRD &= ~(1 << PORTD5);

	//pullup weerstanden
	PORTB |= ((1 << PORTB0) | (1 << PORTB3));
	PORTD |= (1 << PORTD5);

	//enable interrupts voor alle pin changes, werken alleen voor button A en C
	PCMSK0 |= ((1 << PCINT3) | (1 << PCINT0));
}

void writeEEPROM(unsigned int memAddress, uint8_t value) {

	//disable interrupts
	cli();

	//wait for EEPE bit to become 0
	while (EECR & (1 << EEPE))
		;

	//set memory address
	EEAR = memAddress;

	//set value
	EEDR = value;

	//EEPROM master programming enable set
	EECR |= 1 << EEMPE;

	//write 1 to EEPE in EECR to start write
	EECR |= 1 << EEPE;

	//enable interrupts
	sei();
}

uint8_t readEEPROM(unsigned int memAddress) {

	//wait for completion of write
	while (EECR & (1 << EEPE))
		;

	//set memory address
	EEAR = memAddress;

	//enable de read
	EECR |= (1 << EERE);

	//return de read
	return EEDR;
}

int main() {
	initEncoders();
	initUsart();
	initButtons();
	startMotors();

	calibrateMotors();
	return(0);
}
