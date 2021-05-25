#include "usart.h"

void writeChar(char x){
  while (~UCSR1A & (1 << UDRE1));
  UDR1 = x;
}

void writeString(char st[]){
  for(uint8_t i = 0 ; st[i] != 0 ; i++) 
  {
    writeChar(st[i]);
  }
}

void writeInt(int i){
  char buffer[8];
  itoa(i,buffer,10);
  writeString(buffer);
}

void initUsart(){
  UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
  UBRR1 = 103;

  sei();
}