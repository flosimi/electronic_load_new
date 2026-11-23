

#ifndef REMOTE_HH
#define REMOTE_HH

#include <LiquidCrystal_I2C.h>

#include "user_input.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "measurements.hh"
#include "controls.hh"

int remoteMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Measurements& measurements, Controls& controls);

#endif
