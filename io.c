#include "io.h"

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
