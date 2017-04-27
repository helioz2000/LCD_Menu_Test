
// rotary encoder include file by 'jurs' for Arduino Forum
// https://forum.arduino.cc/index.php?topic=318170.msg2202599#msg2202599

#include <Arduino.h>

extern void debug(char *sFmt, ...);

#define ENCODER_A 18
#define ENCODER_B 17

volatile byte state_ISR;
volatile int8_t count_ISR;

volatile unsigned long test_l;

void startTimer2()  // start TIMER2 interrupts
{
  noInterrupts();
  // Timer 2 CTC mode
  TCCR2B = (1<<WGM22) | (1<<CS22)  | (1<<CS20);
  TCCR2A = (1<<WGM21);
  OCR2A = 124;   // 249==500,  124==1000 interrupts per second
                 // 63 ==2000,  31==4000
                 // 15 ==8000,   7==16000
  TIMSK2 = (1<<OCIE2A); // enable Timer 2 interrupts
  interrupts();
}

void stopTimer2() // stop TIMER2 interrupts
{
  noInterrupts();
  TIMSK2 = 0;
  interrupts();
}

int8_t readEncoder()
{ // this function is called within timer interrupt to read one encoder!
  int8_t result=0;
  byte state=state_ISR;
  state= state<<2 | (byte)digitalRead(ENCODER_A)<<1 | (byte)digitalRead(ENCODER_A); 
  state= state & 0xF;   // keep only the lower 4 bits
  /* // next two lines would be code to read 'quarter steps'
  if (state==0b0001 || state==0b0111 || state==0b1110 || state==0b1000) result= -1;
  else if (state==0b0010 || state==0b1011 || state==0b1101 || state==0b0100) result= 1;
  */
  // next two lines is code to read 'full steps'
  if (state==0b0001) result= -1;
  else if (state==0b0010) result= 1;
  state_ISR = state;
  if (result != 0) test_l++;
  return result;
}

void beginEncoder()
{ // active internal pullup resistors on each encoder pin and start timer2
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  readEncoder(); // Initialize start condition
  startTimer2();
  debug("Timer Started %d\n", count_ISR);
  test_l = 0;
}

boolean updateEncoders(int8_t *delta)
{ // read all the 'volatile' ISR variables and copy them into normal variables
  //debug("count ISR: %d\n", count_ISR);
  boolean changeState=false;
  if (count_ISR!=0)
  {
    changeState=true;
    noInterrupts();
    //*delta = count_ISR;
    count_ISR=0;
    interrupts();
  } else {
    //*delta = 0;
  }
  return changeState;
}


ISR(TIMER2_COMPA_vect)  // handling of TIMER2 interrupts
{
  count_ISR += readEncoder();
  
}
