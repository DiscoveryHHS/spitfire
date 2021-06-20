#include "advProximity.h"
#include "irleds.h"
#include "usart.h"
#include "proximitysensors.h"

//sensor vars
volatile uint16_t currBrightnessLevel = 0;
const uint8_t advSensorChannels[] = { 5, 1, 8 }; //adc channels voor prox sensors

//at index 0 left prox sens value, 1 is front prox sens value, 2 is right prox sens value, at FINAL index 0 is the brightness level
volatile uint16_t rawProximitySensorValues[5][4];
volatile uint16_t finalProximitySensorValues[5][4];
volatile uint8_t currSensor = 0;

//brightness levels from small to great for IR Leds
volatile uint16_t brightnessLevels[] = { 380, 390, 400, 410, 420 };
volatile uint8_t brightnessLevelsCounter = 0;

// Special ADC logic for auto drive.
void advInitADC(uint8_t startChannel) {
	ADMUX |= (1 << REFS0); // AVcc with external capacitor on AREF pin
	setADCChannel(startChannel);
	ADCSRA = (1 << ADEN);
	ADCSRA |= ((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADIE)); // Set prescaler at 128 (16Mhz / 200Khz = 80, closed rounded up it 128), and enable interrupts for ADC
}

void advStopADC() {
	ADCSRA &= ~(1 << ADEN);
}

void advSetADCChannel(uint8_t channel) {
	if (channel <= 7) {
		ADCSRB &= ~(1 << MUX5);
		ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	} else {
		ADCSRB |= (1 << MUX5);
		ADMUX = (ADMUX & 0xF0) | ((channel - 8) & 0x0F);
	}
}

void advStartADCConversion() {
	ADCSRA |= (1 << ADSC); //Start conversion
}

//wordt uigevoerd op het moment dat een ADC conversie van een van de proximity sensoren voltooid is, doet telkens 1 cycle van een x aantal verschillende brightness levels voor elke sensor en stopt daarna
void proxSensADCInterruptHandler() {
	//lees waarde uit register
	rawProximitySensorValues[brightnessLevelsCounter][currSensor] = ADC;

	//werk variabele current sensor bij
	currSensor++;

	//werk variabelen bij zoals level van brightness en de huidige uitleesbare sensor
	if (currSensor >= 4) {
		currSensor = 0;
		brightnessLevelsCounter++;
	}

	//if brightnesslevel counter max is behaald moeten alle gegevens geprint worden en gefinalizeerd
	if (brightnessLevelsCounter > 5) {
		brightnessLevelsCounter = 0;
		currSensor = 0;

		//werk final array bij
		finalizeRAWProximityValues();

		//zet IRleds uit
		setIRBrightness(0);

		//stop voor deze cycle
		return;
	}

	//set de nieuwe brightness
	setIRBrightness(brightnessLevels[brightnessLevelsCounter]);

	//bepaal goede richting voor leds
	if (currSensor <= 1) {
		//linker sensors zijn actief dus schijn naar links
		setIRLedsDirection(twoLeftIRLeds);

	} else if (currSensor >= 2) {
		//rechter sensors zijn actief dus schijn naar rechts
		setIRLedsDirection(twoRightIRLeds);
	}

	//start nieuwe ADC conversie
	advSetADCChannel(advSensorChannels[currSensor]);
	advStartADCConversion();
}

//convert from RAW measurments to writable values in final array
void finalizeRAWProximityValues() {
	for (uint8_t i = 0; i < 5; i++) {
		//first write brightness level
		finalProximitySensorValues[i][0] = brightnessLevels[i];

		//cycle trough all measurements and take average of 1 and 2 (middle sensor from left led and middle sensor from right led)
		finalProximitySensorValues[i][1] = rawProximitySensorValues[i][0];
		finalProximitySensorValues[i][2] = ((rawProximitySensorValues[i][1]
				+ rawProximitySensorValues[i][2]) / 2);
		finalProximitySensorValues[i][3] = rawProximitySensorValues[i][3];
	}
}

//starts cycle with interrupts wich reads all sensor values at different brightness settings
void advStartADCProximityCycle() {
	//reset vars
	currSensor = 0;
	brightnessLevelsCounter = 0;

	//set first adc channel
	advSetADCChannel(advSensorChannels[currSensor]);

	//set right brightness for IR leds
	setIRBrightness(brightnessLevels[brightnessLevelsCounter]);

	//set proper direction for leds
	setIRLedsDirection(twoLeftIRLeds);

	//start ADC conversion
	advStartADCConversion();
}

uint16_t* getFinalProxSensValuesPointer() {
	return finalProximitySensorValues;
}
