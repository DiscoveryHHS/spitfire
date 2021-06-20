#include <avr/io.h>

void advInitADC(uint8_t startChannel);
void advStopADC();
void advSetADCChannel(uint8_t channel);
void advStartADCConversion();
void proxSensADCInterruptHandler();
void finalizeRAWProximityValues();
void advStartADCProximityCycle();
uint16_t* getFinalProxSensValuesPointer();

