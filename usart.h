#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeChar(char);
void writeString(char[]);
void writeInt(int);
void initUsart();