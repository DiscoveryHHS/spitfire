#include <avr/io.h>

void initADC(uint8_t startChannel);
void stopADC();
void setADCChannel(uint8_t channel);
void startADCConversion();
