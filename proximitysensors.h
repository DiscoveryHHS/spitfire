#include <avr/io.h>

void writeAllProximityValues();
void proxSensADCInterruptHandler();
void finalizeRAWProximityValues();
void initIRLeds();
void startADCProximityCycle();
uint16_t* getFinalProxSensValuesPointer();
