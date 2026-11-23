#include "encoder.hh"

Encoder * Encoder::instance;

void Encoder::begin(){
  instance = this;                         //before the interrupts are attached: the ISRs dereference it
  pinMode(ENCODER_A, INPUT_PULLUP);        //pull-ups first, so floating pins don't generate edges
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), isrRotation, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrButton, FALLING);
}


void Encoder::reset(){
  uint8_t sreg = SREG;                     //counter is 16 bit: the ISR must not run in the middle of the access
  cli();
  counter = 0;
  buttonPressed = false;
  SREG = sreg;
}

void Encoder::isrRotation(){
  instance->handleInterruptRotation();
}

void Encoder::isrButton(){
  instance->handleInterruptButton();
}

void Encoder::handleInterruptRotation(){
  static unsigned long lastInterruptTimeRotation = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTimeRotation > 5){
    if(digitalRead(ENCODER_B) == LOW) 
      counter--;
    else 
      counter++;
  }

  lastInterruptTimeRotation = interruptTime;
}

void Encoder::handleInterruptButton(){
  static unsigned long lastInterruptTimeButton = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 100ms, assume it's a bounce and ignore
  if(interruptTime - lastInterruptTimeButton > 100)
    buttonPressed = true;
  
  lastInterruptTimeButton = interruptTime;
}


bool Encoder::wasButtonPressed(){
  uint8_t sreg = SREG;                     //read and clear atomically, a press arriving in between would be lost
  cli();
  bool pressed = buttonPressed;
  buttonPressed = false;                   //reset buttonPressed flag
  SREG = sreg;
  return pressed;
}

int Encoder::takeRotation(){               //read and clear the rotation counter atomically (no lost steps)
  uint8_t sreg = SREG;
  cli();
  int value = counter;
  counter = 0;
  SREG = sreg;
  return value;
}

int Encoder::rotation(){
  uint8_t sreg = SREG;
  cli();
  int value = counter;
  SREG = sreg;
  return value;
}