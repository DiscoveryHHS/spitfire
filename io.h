#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

void initButtons();
void buttonsInterruptHandler();
bool obstacleAvoidanceModeIsEnabled();
