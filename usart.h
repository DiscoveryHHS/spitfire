#include <avr/io.h>

void writeChar(char);
void writeString(char[]);
void writeInt(int);
void initUsart();
void usartSplitToCommand(char[], uint8_t, char[3], char[4][8]);
