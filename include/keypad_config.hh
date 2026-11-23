
#ifndef KEYPAD_HH
#define KEYPAD_HH

#include <Keypad.h>
#include "defines.hh"

enum SpecialKeys{
    LoadOnOff = -4,
    Menu,
    Enter,
    Delete
};

extern char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS];
extern uint8_t rowPins[KEYPAD_ROWS];
extern uint8_t colPins[KEYPAD_COLS];

#endif