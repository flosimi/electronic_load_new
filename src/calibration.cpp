

#include <EEPROM.h>
#include "calibration.hh"


int limit(int& value){
  if(value < 0)
    value = 0;
  else if(value > 255)
    value = 255;
  return value;
}


//blank EEPROM reads 255 - use the neutral value instead
static int readOr(int address, int neutral){
  int v = EEPROM.read(address);
  return v == 255 ? neutral : v;
}

void AdcCalibration::readFromEEPROM(){
  voltageMultiplier = readOr(EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL, 128);
  voltageOffset = readOr(EEPROM_ADDRESS_ADC_U_OFFSET_CAL, 128);
  currentMultiplier = readOr(EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL, 128);
  currentOffset = readOr(EEPROM_ADDRESS_ADC_I_OFFSET_CAL, 0);
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
  setCurrentMultiplier = readOr(EEPROM_ADDRESS_DAC_MULTIPLIER_CAL, 128);
  setCurrentOffset = readOr(EEPROM_ADDRESS_DAC_OFFSET_CAL, 128);
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