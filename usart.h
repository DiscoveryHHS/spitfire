#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeChar(char);
void writeString(char[]);
void writeInt(int);
void initUsart();
void usartSplitToCommand(char[], uint8_t, char[3], char[4][8]);
