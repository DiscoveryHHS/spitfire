#include <avr/io.h>
#include <stdbool.h>

void initButtons();
void buttonsInterruptHandler();
bool obstacleAvoidanceModeIsEnabled();
