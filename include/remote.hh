

#ifndef REMOTE_HH
#define REMOTE_HH

#include <LiquidCrystal_I2C.h>

#include "user_input.hh"
#include "encoder.hh"
#include "keypad_config.hh"
#include "measurements.hh"
#include "controls.hh"

//Serial remote control (9600 baud, commands end with a newline):
//  CC <A> | CP <W> | CR <ohm>   select the mode and set the value
//  ON | OFF                     load on / off
//  MEAS?                        reply "M;volt;amp;watt;temp;on;mode;setpoint"
//  HELP                         list the commands
//  EXIT                         leave the remote mode (load off)
//Replies: OK / OK <mode> <value> / ERR. The encoder button switches the load OFF.
int remoteMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls);

#endif
