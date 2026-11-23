#include "encoder.hh"

Encoder * Encoder::instance;

void Encoder::begin(){
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), isrRotation, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_BUTTON), isrButton, FALLING);
  instance = this;
}


void Encoder::reset(){
  counter = 0;
  buttonPressed = false;
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
  if(buttonPressed){
    buttonPressed = false; //reset buttonPressed flag
    return true;
  }
  else
    return false;
}

int Encoder::rotation(){
  return counter;
}