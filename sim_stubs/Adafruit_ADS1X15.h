// SIMULATION STUB - behaves like a perfect load:
//  - input voltage is a fixed SIM_INPUT_VOLTAGE
//  - measured current is exactly the current commanded through the DAC (closed loop through the real scaling constants)
// Assumes neutral calibration, see EEPROM.h stub.
#ifndef ADC_STUB_H
#define ADC_STUB_H
#include <Arduino.h>
#include "defines.hh"
#include "Adafruit_MCP4725.h"

#ifndef SIM_INPUT_VOLTAGE
#define SIM_INPUT_VOLTAGE 24.0
#endif

typedef enum { GAIN_TWOTHIRDS, GAIN_ONE, GAIN_TWO, GAIN_FOUR, GAIN_EIGHT, GAIN_SIXTEEN } adsGain_t;

class Adafruit_ADS1115 {
  public:
    bool begin(uint8_t){ return true; }
    void setGain(adsGain_t){}
    int16_t readADC_Differential_2_3(){
      return (int16_t)(SIM_INPUT_VOLTAGE / 100.0 / 0.512 * 32768.0);
    }
    int16_t readADC_SingleEnded(uint8_t){
      float amps = simDacValue() * (DAC_VREF / 4096.0) / DAC_VOLTS_PER_AMP;
      return (int16_t)(amps * CURRENT_SHUNT_OHM / ADC_CURRENT_FULL_SCALE * 32768.0);
    }
};
#endif
