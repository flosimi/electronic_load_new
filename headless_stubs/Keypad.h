// HEADLESS STUB - no keypad connected: getKey() never returns a key and no pin is touched.
// Only used by the "headless" PlatformIO environment.
#ifndef HEADLESS_KEYPAD_STUB_H
#define HEADLESS_KEYPAD_STUB_H
#include <Arduino.h>

#define NO_KEY '\0'
#define makeKeymap(x) ((char*)(x))

class Keypad {
  public:
    Keypad(char*, byte*, byte*, byte, byte){}
    char getKey(){ return NO_KEY; }
};
#endif
