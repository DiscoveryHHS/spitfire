/*
 * encoder status wordt als volgt bijgehouden:
 *         linkerdeel               |           rechterdeel
 * oude deel     |   nieuwe deel    |   oude deel   |   nieuwe deel
 * pin A | pin B | pin A   |  pin B | pin A | pin B | pin A | pin B
 */

#include "encoders.h"

//encoder ticks
volatile uint64_t leftEncoderTicksCounter = 0;
volatile uint64_t rightEncoderTicksCounter = 0;

volatile double leftMotorRatio = 1;
volatile double rightMotorRatio = 1;

//current states of encoders, at index 0 status of pcint0 is kept, at index 7 status of pcint7 is kept
volatile uint8_t encoderStatus = 0;
volatile uint8_t pcintStatus = 0;

void resetEncoderTicks() {
	leftEncoderTicksCounter = 0;
	rightEncoderTicksCounter = 0;
}

void initEncoders() {
	//rechter encoders op input
	DDRE &= ~(1 << PORTE6);
	DDRF &= ~(1 << PORTF0);

	//linker encoder op input
	DDRB &= ~(1 << PORTB4);
	DDRE &= ~(1 << PORTE2);

	//enable pullup weerstanden
	PORTB |= 1 << PORTB4;
	PORTE |= ((1 << PORTE2) | (1 << PORTE6));
	PORTF |= 1 << PORTF0;

	//enable interrupts voor pin change 0...7
	PCICR |= 1 << PCIE0;

	//enable interrupts voor pin change 4
	PCMSK0 |= (1 << PCINT4);

	//bij elke logische verandering een interrupt op int6
	EICRB |= (1 << ISC60);

	//enable interrupts voor int6
	EIMSK |= 1 << INT6;

	//clear interrupt flag
	PCIFR |= (1 << PCIF0);
}

void leftEncoderInterruptHandler() {

	//check wich pcint is generated
	//pcint 4 (left encoder)
	if ((pcintStatus ^ ((PINB & (1 << PINB4)) >> 1))) {
		//get new status for pin A
		encoderStatus |= (((PINB | (1 << PINB4) ^ (PINE | (1 << PINE2)))) << 5);

		//get new status for pin B
		encoderStatus |= ((PINE | (1 << PINE2)) << 4);

		//check if clockwise rotation
		if ((~(encoderStatus & (1 << 7)) & (encoderStatus & (1 << 6))
				& ~(encoderStatus & (1 << 5)) & ~(encoderStatus & (1 << 4)))) {
			leftEncoderTicksCounter++;
		}

		//clear old status an bitshift linkerdeel 2 to left
		encoderStatus &= ~((1 << 7) | (1 << 6));
		encoderStatus |= (encoderStatus & ((1 << 7) | (1 << 6)));

		//clear moved old status
		encoderStatus &= ~((1 << 5) | (1 << 4));

		//update pcint status
		pcintStatus ^= (1 << 4);
	}

	//pcint 3 (button A), only when from high to low (wich means button is pressed)
	if (~PINB & (1 << PINB3)) {
		//small debounce
		_delay_ms(10);
		if (~PINB & (1 << PINB3)) {
		}
	}

	//pcint 0 (button C), only when from high to low (wich means button is pressed)
	if (~PINB & (1 << PINB0)) {
		//small debounce
		_delay_ms(10);
		if (~PINB & (1 << PINB0)) {
		}
	}
}

void rightEncoderInterruptHandler() {

	//get new status for pin A
	encoderStatus |= (((PINE | 1 << PINE6) ^ (PINF | (1 << PINF0))) << 1);

	//get new status for pin B
	encoderStatus |= (PINF | (1 << PINF0));

	//check of kloksgewijze rotatie waarbij 1 tick erbij gaat
	if (~(encoderStatus & (1 << 3)) & (encoderStatus & (1 << 2))
			& ~(encoderStatus & (1 << 1)) & ~(encoderStatus & (1 << 0))) {
		rightEncoderTicksCounter++;
	}

	//clear old status an bitshift rechterdeel 2 to left
	encoderStatus &= ~((1 << 3) | (1 << 2));
	encoderStatus |= (encoderStatus & ((1 << 3) | (1 << 2)));

	//clear moved old status
	encoderStatus &= ~((1 << 1) | (1));
}

void calibrateMotors() {

	//wacht even en reset encoder ticks
	_delay_ms(1000);
	resetEncoderTicks();

	//stuk vooruit
	setLeftSpeed(DEFAULTSPEED, 1);
	setRightSpeed(DEFAULTSPEED, 1);
	_delay_ms(1000);

	//stuk achteruit
	setLeftSpeed(DEFAULTSPEED, 0);
	setRightSpeed(DEFAULTSPEED, 0);
	_delay_ms(1000);

	//draai linksom
	setLeftSpeed(DEFAULTSPEED, 0);
	setRightSpeed(DEFAULTSPEED, 1);
	_delay_ms(500);

	//draai rechtsom
	setLeftSpeed(DEFAULTSPEED, 1);
	setRightSpeed(DEFAULTSPEED, 0);
	_delay_ms(500);

	//stop beide motoren
	setLeftSpeed(0, 1);
	setRightSpeed(0, 1);

	//bereken motor ratio's
	if (leftEncoderTicksCounter > rightEncoderTicksCounter) {
		//write to memory address in EEPROM
		//EEPROM.write(0, ((((double) rightEncoderTicksCounter) / ((double) leftEncoderTicksCounter)) * 0.98);
	} else {
		//write to memory address in EEPROM
		//EEPROM.write(1, ((((double) leftEncoderTicksCounter) / ((double) rightEncoderTicksCounter)) * 0.98));
	}

	//reset encoders
	resetEncoderTicks();
}
