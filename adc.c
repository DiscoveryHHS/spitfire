#include "adc.h"

void initADC(uint8_t startChannel) {
	ADMUX |= (1 << REFS0); // AVcc with external capacitor on AREF pin
	setADCChannel(startChannel);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= ((1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2) | (1 << ADIE)); // Set prescaler at 128 (16Mhz / 200Khz = 80, closed rounded up it 128), and enable interrupts for ADC
}

void stopADC() {
	ADCSRA &= ~(1 << ADEN);
}

void setADCChannel(uint8_t channel) {
	if (channel <= 7) {
		ADCSRB &= ~(1 << MUX5);
		ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	} else {
		ADCSRB |= (1 << MUX5);
		ADMUX = (ADMUX & 0xF0) | ((channel - 8) & 0x0F);
	}
}

void startADCConversion() {
	ADCSRA |= (1 << ADSC); //Start conversion
}

uint16_t readADC()
{
	return ADC;
}
