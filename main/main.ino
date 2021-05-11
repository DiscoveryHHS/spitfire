#define F_CPU 16000000

void initUsart();
void writeChar(char);

void motorStart();
void motorStop();
void motorRightSpeed(uint8_t);
void motorLeftSpeed(uint8_t);

ISR(USART1_RX_vect) {
  char send = UDR1;

  switch(send) {
    case 'w':
      motorStart();
      motorRightSpeed(255);
      motorLeftSpeed(255);
      break;

    case 'a':
      motorRightSpeed(180);
      motorLeftSpeed(40);
      break;

    case 's':
      motorStop();
      break;

    case 'd':
      motorRightSpeed(40);
      motorLeftSpeed(180);
      break;
  }
}

void setup() {
  initUsart();
  motorRightSpeed(250);
  motorLeftSpeed(250);
}

void loop() {
}

void initUsart() {
  UCSR1B = (1 << RXCIE1) | (1 << RXEN1) | (1 << TXEN1);
  UBRR1 = 103;
  sei();
}

void writeChar(char x) {
  while (~UCSR1A & (1 << UDRE1));
  UDR1 = x;
}

void motorStart() {
  //set prescaler naar 64
  TCCR1B |= ((1 << CS11) | (1 << CS10));

  //mode 5 pwm mode
  TCCR1A |= 1 << WGM10;
  TCCR1B |= 1 << WGM12;

  //clear bij output compare match
  TCCR1A |= (1 << COM1A1);
  TCCR1A |= (1 << COM1B1);

  //set output van timers als output
  DDRB |= ((1 << PORTB5) | (1 << PORTB6)); 
}

void motorStop() {
  TCCR1B = 0;
  TCCR1A = 0;
}

void motorRightSpeed(uint8_t speed) {
  OCR1A = speed;
}

void motorLeftSpeed(uint8_t speed) {
  OCR1B = speed;
}
