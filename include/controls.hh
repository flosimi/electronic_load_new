
#ifndef CONTROLS_HH
#define CONTROLS_HH

#include <Adafruit_MCP4725.h>
#include "measurements.hh"
#include "calibration.hh"
#include "defines.hh"


class Controls{
  public:
    Adafruit_MCP4725 dac;
    DacCalibration calibration;
    Measurements& measurements;   //copy of measurements
    void fanControll();
    void loadOn(LiquidCrystal_I2C& lcd);
    void loadOff(LiquidCrystal_I2C& lcd);
    void loadOnOffToggle(LiquidCrystal_I2C& lcd);
    void regulateCurrent(float setCurrent);
    void regulatePower(float setPower);
    void regulateResistance(float setResistance);
    bool isLoadOn();

    Controls(Measurements& measurements_);
  private:
    int fanspeed;
    int lowTemperatureThreshold = 30;   //fan turns on after reaching this temperature
    int highTemperatureThreshold = 45;  //fan reaches full speed at this temperature
    bool loadIsOn = false;
};


#endif