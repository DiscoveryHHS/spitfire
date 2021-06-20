/*
 * Deze library is volledig ontworpen en geprogrammeerd door Stan Albada Jelgersma met studentnummer 20166893
 * Deze library is uitsluitend bedoeld voor de ZUMO robot met als basis de AtMEGA32u4 microcontroller
 * Datum: 11-5-2021
 */
#include <avr/io.h>

void setLeftSpeed(uint16_t, int);
void setRightSpeed(uint16_t, int);
void startMotors();
void stopMotors();
void obstacleAvoider();
uint16_t getDefaultSpeed();
