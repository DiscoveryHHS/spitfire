#include "io.h"

volatile bool obstacleAvoidModeEnabled = 0;

// Initializes A and B Buttons.
void initButtons() {
	// DDR Registers for all buttons
	DDRB &= ~((1 << PORTB3) | (1 << PORTB0));
	DDRD &= ~(1 << PORTD5);

	// Enable pull-up on buttons
	PORTB |= ((1 << PORTB0) | (1 << PORTB3));
	PORTD |= (1 << PORTD5);

	// Enable interrupts for button A and C
	PCMSK0 |= ((1 << PCINT3) | (1 << PCINT0));
}

void buttonsInterruptHandler() {
	//pcint 3 (button A), only when from high to low (wich means button is pressed)
	if (~PINB & (1 << PINB3)) {
		// Do stuff on button A pressed, in this case enable or disable the obstacle avoidance mode
		obstacleAvoidModeEnabled = !obstacleAvoidModeEnabled;

		if (!obstacleAvoidModeEnabled) {
			stopADC();
			//set speeds to 0
			setLeftSpeed(0, 1);
			setRightSpeed(0, 1);
		} else {
			initADC(1);
		}
		//small debounce
		_delay_ms(15);
	}

	//pcint 0 (button C), only when from high to low (wich means button is pressed)
	if (~PINB & (1 << PINB0)) {
		// Do stuff on button C pressed, in this case this is a force stop for the motors
		stopMotors();
		//small debounce
		_delay_ms(15);
	}
}

//if set to bool 1 or true, obstacle avoid mode is enabled, else disabled
void setObstacleAvoidMode(bool enable) {
	obstacleAvoidModeEnabled = enable;
}

bool obstacleAvoidanceModeIsEnabled() {
	return obstacleAvoidModeEnabled;
}
