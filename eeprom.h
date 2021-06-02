#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

void writeEEPROM(unsigned int, uint8_t);
uint8_t readEEPROM(unsigned int);
