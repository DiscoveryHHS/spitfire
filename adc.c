#include "adc.h"

void initADC(uint8_t startChannel) {
	ADMUX |= (1 << REFS0); // AVcc with external capacitor on AREF pin
	setADCChannel(startChannel);
	ADCSRA = (1 << ADEN);
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // Set prescaler at 128 (16Mhz / 200Khz = 80, closest rounded up it 128)
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

uint16_t readADCValue() {
	ADCSRA |= (1 << ADSC); //Start conversion
	while ( ADCSRA & (1 << ADSC))
		; // Wait for flag
	uint16_t value = ADC;
	return value;
}
