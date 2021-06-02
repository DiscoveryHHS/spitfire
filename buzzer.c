#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "buzzer.h"

void startBuzzerTimer()
{
	//prescaler op /8
	TCCR4B = 1 << CS43 | 1 << CS40;

	//pin van buzzer is output
	DDRD |= 1 << PORTD7;

	//zet timer 4 op clear compare match OC4D pin en fast pwm mode
	TCCR4C = ((1 << COM4D1) | (0 << COM4D0) | (1 << PWM4D));
}

void stopBuzzerTimer()
{
	TCCR4B &= ~((1 << CS43) | (1 << CS40));
	DDRD &= ~(1 << PORTD7);
	TCCR4C &= ~((1 << COM4D1) | (0 << COM4D0) | (1 << PWM4D));
}

void playBuzzerStartupSound()
{
	for(uint8_t i = 0; i < 3; i++)
	{
		OCR4D = 50;
		_delay_ms(75);
		OCR4D =  0;
		_delay_ms(250);
	}
	_delay_ms(500);
	OCR4D = 50;
	_delay_ms(250);
	OCR4D = 0;
}

void setBuzzerDutyCycle(uint8_t dutyCycle)
{
	OCR4D = dutyCycle;
}