

#ifndef BATTERY_HH
#define BATTERY_HH

#include <LiquidCrystal_I2C.h>

#include "set_value.hh"
#include "defines.hh"

enum BatteryChangedVariable{
  DischargeCurrent,
  CutoffVoltage
};


class Battery{
  public:
    float capacity = 0;         //in mAh
    uint32_t dischargeTime = 0; //in seconds
    SetValue cutoffVoltage;     //in Volts
    SetValue dischargeCurrent;  //in Amps
    BatteryChangedVariable changedVariable;
    
    Battery();
    void displayCapacity(LiquidCrystal_I2C& lcd);
  private:
    
};

#endif