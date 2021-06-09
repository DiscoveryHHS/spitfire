/*
 * Deze library is volledig ontworpen en geprogrammeerd door Stan Albada Jelgersma met studentnummer 20166893
 * Deze library is uitsluitend bedoeld voor de ZUMO robot met als basis de AtMEGA32u4 microcontroller
 * Datum: 11-5-2021
 */

#include "motors.h"
#include "encoders.h"
#include "proximitysensors.h"

volatile uint16_t DEFAULTSPEED = 20000;

//set de snelheid van de linker rupsband, eerst timer 1 starten
void setLeftSpeed(uint16_t leftSpeed, int driveForward) {
	if (driveForward == 0) {
		PORTB |= 1 << PORTB2;
	} else {
		PORTB &= ~(1 << PORTB2);
	}
	OCR1B = leftSpeed * getLeftMotorRatio();
}

//set de snelheid van de rechter rupsband, eerst timer 1 starten
void setRightSpeed(uint16_t rightSpeed, int driveForward) {
	if (driveForward == 0) {
		PORTB |= 1 << PORTB1;
	} else {
		PORTB &= ~(1 << PORTB1);
	}
	OCR1A = rightSpeed * getRightMotorRatio();
}

//start timer 1 die beide motoren met pwm aanstuurt
void startMotors() {
	//mode 14 pwm mode, prescaler naar 1 (geen prescaler), clear bij output compare match
	TCCR1A = ((1 << WGM11) | (1 << COM1A1) | (1 << COM1B1));
	TCCR1B = ((1 << CS10) | (1 << WGM12) | (1 << WGM13));

	//ICR (bepaalt top) op max
	ICR1 = 0xFFFF;

	//set output van timers als output
	DDRB |= ((1 << PORTB5) | (1 << PORTB6));

	//set vooruit en achteruit pins als output
	DDRB |= ((1 << PORTB1) | (1 << PORTB2));

	//default in vooruit mode
	PORTB &= ~((1 << PORTB1) | (1 << PORTB2));
	
	//flags en mask registers naar 0 voor geen interrupts
	TIMSK1 = 0;
	TIFR1 = 0;
}

//stop de timer 1, de motoren werken hierna niet meer
void stopMotors() {
	TCCR1B = 0;
	TCCR1A = 0;
	OCR1A = 0;
	OCR1B = 0;
}

void obstacleAvoider(){
	static uint16_t *proxSens;
	
	//verkrijg proximity sensor data
	proxSens = getFinalProxSensValuesPointer();
	
	//verkrijg prox sensor data
	proxSens = getFinalProxSensValuesPointer();
	
	//check of rechtdoor vrij is
	if(((*(proxSens + (4 * 4) + 2)) < 550))
	{
		//rechtdoor is niet vrij, vergelijk rechts met links, kies degene waar de meeste ruimte is
		if(((*(proxSens + (4 * 4) + 1))) >= ((*(proxSens + (4 * 4) + 3))))
		{
			//aan de linkerkant is meer ruimte, draai naar links
			setLeftSpeed(DEFAULTSPEED, 0);
			setRightSpeed(DEFAULTSPEED, 1);
		}
		else
		{
			//aan de rechterkant is meer ruimte, draai naar rechts
			setLeftSpeed(DEFAULTSPEED, 1);
			setRightSpeed(DEFAULTSPEED, 0);
		}
	}
	
	//check of links vrij is
	else if(((*(proxSens + (1 * 4) + 1)) < 550))
	{
		//links is niet vrij, draai naar rechts
		setLeftSpeed(DEFAULTSPEED, 1);
		setRightSpeed(DEFAULTSPEED, 0);
	}
	
	//check of rechts vrij is
	else if(((*(proxSens + (1 * 4) + 3)) < 550))
	{
		//rechts is niet vrij, draai naar links
		setLeftSpeed(DEFAULTSPEED, 0);
		setRightSpeed(DEFAULTSPEED, 1);
	}
	
	else
	{
		//rij normaal vooruit
		setLeftSpeed(DEFAULTSPEED, 1);
		setRightSpeed(DEFAULTSPEED, 1);
	}
}

uint16_t getDefaultSpeed(){
	return DEFAULTSPEED;
}
