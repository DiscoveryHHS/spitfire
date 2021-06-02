#include <avr/io.h>
#include <avr/interrupt.h>

void writeEEPROM(unsigned int, uint8_t);
uint8_t readEEPROM(unsigned int);
