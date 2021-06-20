#include "eeprom.h"
#include <stdbool.h>
#include <avr/interrupt.h>

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

bool checkEEPROMAddressAvailable(unsigned int memAddress) {
	if (readEEPROM(memAddress) != 0) {
		return 0;
	}
	return 1;
}
