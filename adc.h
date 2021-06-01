#include <avr/io.h>

/*
 * Example usage:
 *
 * 	initADC(8);
 *
 *	while (1) {
 *		uint16_t value = readADCValue();
 *		writeInt(value);
 *		writeChar('\n');
 *		writeChar(13);
 *		_delay_ms(200);
 *	}
 *
 *
 */

void initADC(uint8_t startChannel);
void stopADC();
void setADCChannel(uint8_t channel);
uint16_t readADCValue();
