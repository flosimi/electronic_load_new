

#ifndef ENCODER_HH
#define ENCODER_HH

#include <Arduino.h>
#include "defines.hh"

class Encoder{
  private:
    volatile int counter = 0;
    volatile bool buttonPressed = false;
    static Encoder * instance;
    static void isrRotation();
    static void isrButton();
    void handleInterruptRotation ();
    void handleInterruptButton ();

  public:
    void begin ();
    void reset();
    bool wasButtonPressed();
    int rotation();
};

#endif