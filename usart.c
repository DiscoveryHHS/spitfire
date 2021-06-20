#include "usart.h"

#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeChar(char x) {
	while (~UCSR1A & (1 << UDRE1))
		;
	UDR1 = x;
}

void writeString(char st[]) {
	for (uint8_t i = 0; st[i] != 0; i++) {
		writeChar(st[i]);
	}
}

void writeInt(int i) {
	char buffer[8];
	itoa(i, buffer, 10);
	writeString(buffer);
}

void initUsart() {
	UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
	UBRR1 = 103;

	sei();
}

/*
 * Split incoming buffer into a command, and a array of params strings.
 */
void usartSplitToCommand(char usartBuffer[], uint8_t usartBufferLength,
		char command[3], char params[4][8]) {
	command[0] = usartBuffer[0];
	command[1] = usartBuffer[1];
	command[2] = '\0';

	uint8_t paramsLength = 0;
	uint8_t paramTempLength = 0;
	uint8_t skippedChars = 0;

	uint8_t i;

	for (i = 3; i < usartBufferLength; i++) {
		if ((usartBuffer[i] >= 48 && usartBuffer[i] <= 57)
				|| (usartBuffer[i] >= 65 && usartBuffer[i] <= 90)
				|| (usartBuffer[i] >= 97 && usartBuffer[i] <= 122)) { // Check if the char is A-Z,a-z,0-9
			params[paramsLength][paramTempLength] = usartBuffer[i];
			paramTempLength++;
			skippedChars = 0;
		} else {
			if (skippedChars == 0) {
				params[paramsLength][paramTempLength] = '\0';
				paramTempLength = 0;
				paramsLength++;
			}
			skippedChars++;
		}
	}
}
