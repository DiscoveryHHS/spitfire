#include <avr/io.h>
#include "IRLeds.h"

//initializes infrared lights at front and middle, are connected to timer 3 PWM
void startIRLeds()
{
	//set as output
	DDRC |= 1 << PORTC6;
	
	//pwm mode 14 (fast PWM)
	TCCR3A = ((1 << COM3A1) | (1 << WGM31));
	
	/*
	we moeten 38 kHz infrarood signalen uitzenden, clock is 16mHz en timer kan maximaal tot 65535 tellen (timer 3 is 16 bit)
	zonder prescaler overflowt timer 3 16000000 / 65536 = ongeveer 244 keer, wat dus veel te weinig is voor de 38 kHz die we moeten behalen, dus ICR3 moet aangepast worden
	aangezien ICR3 de top bepaalt voor timer 3. zonder prescaler moet ICR3 16000000 / 38000 = 421 zijn.
	*/
	
	//geen prescaler en pwm mode 14
	TCCR3B = ((1 << WGM32) | (1 << WGM33) | (1 << CS30));
	
	//set top
	ICR3 = 421;
	
	//set standard brightness to 0
	OCR3A = 0;
	
	//register to set IR Leds direction to output
	DDRF |= 1 << PORTF6;
}

//set de brightness van de IR leds
void setIRBrightness(uint16_t brightness)
{
	OCR3A = brightness;
}

void setIRLedsDirection(enum Direction dir)
{
	//only 2 of the 4 onboard leds can shine simultaneausly, set pf6 to shine the 2 right side Leds, reset pf6 to shine 2 left side leds
	if(dir == twoLeftIRLeds)
	{
		PORTF &= ~(1 << PORTF6);
	}
	
	else if(dir == twoRightIRLeds)
	{
		PORTF |= (1 << PORTF6);
	}
} 