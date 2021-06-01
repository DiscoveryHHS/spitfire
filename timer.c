#include "timer.h"

void initTimer0() {
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	OCR0A = 255;
	TIMSK0 |= (1 << OCIE0A);
}
