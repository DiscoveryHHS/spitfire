#include <avr/io.h>
#include <util/delay.h>

void startBuzzerTimer();
void stopBuzzerTimer();
void playBuzzerStartupSound();
void setBuzzerDutyCycle(uint8_t);