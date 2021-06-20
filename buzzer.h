#include <avr/io.h>
#include <util/delay.h>

void startBuzzerTimer();
void stopBuzzerTimer();
void setBuzzerDutyCycle(uint8_t);
void playBuzzerStartupSound();
