// SIMULATION STUB - remembers the last value written to the DAC so the ADC stub can "measure" it.
#ifndef DAC_STUB_H
#define DAC_STUB_H
#include <Arduino.h>

inline uint16_t& simDacValue(){ static uint16_t v = 0; return v; }

class Adafruit_MCP4725 {
  public:
    bool begin(uint8_t){ return true; }
    bool setVoltage(uint16_t output, bool){ simDacValue() = output; return true; }
};
#endif
