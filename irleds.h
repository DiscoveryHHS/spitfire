#include <avr/io.h>

enum Direction {twoLeftIRLeds, twoRightIRLeds};
void startIRLeds();
void setIRBrightness(uint16_t brightness);
void setIRLedsDirection(enum Direction);
