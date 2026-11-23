
#include "init.hh"


void pinInit(){
  pinMode(THERMISTOR, INPUT);
  pinMode(FAN, OUTPUT);
  pinMode(OUTPUT_OFF, OUTPUT);
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON, INPUT_PULLUP);
}

