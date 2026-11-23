

#include <EEPROM.h>
#include "calibration.hh"


int limit(int& value){
  if(value < 0)
    value = 0;
  else if(value > 255)
    value = 255;
  return value;
}


void AdcCalibration::readFromEEPROM(){
  voltageMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL);
  voltageOffset = EEPROM.read(EEPROM_ADDRESS_ADC_U_OFFSET_CAL);
  currentMultiplier = EEPROM.read(EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL);
  currentOffset = EEPROM.read(EEPROM_ADDRESS_ADC_I_OFFSET_CAL);
}

void AdcCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL, voltageMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_U_OFFSET_CAL, voltageOffset);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL, currentMultiplier);
  EEPROM.write(EEPROM_ADDRESS_ADC_I_OFFSET_CAL, currentOffset);
}

int AdcCalibration::getVoltageMultiplier(){return voltageMultiplier;}
int AdcCalibration::getVoltageOffset(){return voltageOffset;}
int AdcCalibration::getCurrentMultiplier(){return currentMultiplier;}
int AdcCalibration::getCurrentOffset(){return currentOffset;}

void AdcCalibration::setVoltageMultiplier(int cal){
  voltageMultiplier = limit(cal);
}
void AdcCalibration::setVoltageOffset(int cal){
  voltageOffset = limit(cal);
}
void AdcCalibration::setCurrentMultiplier(int cal){
  currentMultiplier = limit(cal);
}
void AdcCalibration::setCurrentOffset(int cal){
  currentOffset = limit(cal);
}


void DacCalibration::readFromEEPROM(){
  setCurrentMultiplier = EEPROM.read(EEPROM_ADDRESS_DAC_MULTIPLIER_CAL);
  setCurrentOffset = EEPROM.read(EEPROM_ADDRESS_DAC_OFFSET_CAL);
}


void DacCalibration::writeToEEPROM(){
  EEPROM.write(EEPROM_ADDRESS_DAC_MULTIPLIER_CAL, setCurrentMultiplier);
  EEPROM.write(EEPROM_ADDRESS_DAC_OFFSET_CAL, setCurrentOffset);
}

int DacCalibration::getSetCurrentMultiplier(){return setCurrentMultiplier;}
int DacCalibration::getSetCurrentOffset(){return setCurrentOffset;}

void DacCalibration::setSetCurrentMultiplier(int cal){
  setCurrentMultiplier = limit(cal);
}
void DacCalibration::setSetCurrentOffset(int cal){
  setCurrentOffset = limit(cal);
}