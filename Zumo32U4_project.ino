#include "zumoAansluitingen.h";

/*
 * op OC1A zit rechtermotor voorkant aangesloten
 * op OC1B zit linkermotor voorkant aangesloten
 */

//set de snelheid van de linker rupsband, eerst timer 1 starten
void setLeftSpeed(uint8_t leftSpeed)
{
  OCR1B = leftSpeed;
}

//set de snelheid van de rechter rupsband, eerst timer 1 starten
void setRightSpeed(uint8_t rightSpeed)
{
  OCR1A = rightSpeed;
}

/*
 * start timer 1 die beide motoren met pwm aanstuurt
 */
void startTimer1PWM()
{
  //set prescaler naar 64
  TCCR1B |= ((1 << CS11) | (1 << CS10));

  //mode 5 pwm mode
  TCCR1A |= 1 << WGM10;
  TCCR1B |= 1 << WGM12;

  //clear bij output compare match
  TCCR1A |= (1 << COM1A1);
  TCCR1A |= (1 << COM1B1);

  //set output van timers als uitput
  DDRB |= ((1 << PORTB5) | (1 << PORTB6));
}

//stop de timer 1, de motoren werken hierna niet meer
void stopTimer1PWM()
{
  TCCR1B = 0;
  TCCR1A = 0;
}

void setup() {
  startTimer1PWM();

  //beide motoren op snelheid 0
  setLeftSpeed(0);
  setRightSpeed(0);
}

void loop() {
  uint8_t teller = 0;
  bool left = false;

  while(1)
  {
    if(left)
    {
      setLeftSpeed(teller);
      teller++;
      if(teller == 255)
      {
        teller = 0;
        setLeftSpeed(teller);
        left = false;
      }
      delay(25);
    }
    if(!left)
    {
      setRightSpeed(teller);
      teller++;
      if(teller == 255)
      {
        teller = 0;
        setRightSpeed(teller);
        left = true;
      }
      delay(25);
    }
  }
}
