// SIMULATION STUB - returns "neutral" calibration values so measurements are not skewed by a blank EEPROM
#ifndef EEPROM_STUB_H
#define EEPROM_STUB_H
#include <Arduino.h>
#include "defines.hh"

struct EEPROMClass {
  uint8_t read(int address){
    if(address == EEPROM_ADDRESS_ADC_U_OFFSET_CAL || address == EEPROM_ADDRESS_ADC_I_OFFSET_CAL)
      return 0;
    return 128;   //multipliers (about x1.00) and the DAC offset (128 = no offset)
  }
  void write(int, uint8_t){}
};
static EEPROMClass EEPROM;
#endif
