#include <avr/io.h>
#include "irleds.h"
#include "adc.h"
#include "usart.h"

void writeAllProximityValues();
void proxSensADCInterruptHandler();
void finalizeRAWProximityValues();
void initIRLeds();
void startADCProximityCycle();
