

#ifndef CALIBRATION_HH
#define CALIBRATION_HH

#include "defines.hh"

/**
 * @brief contains calibration values for the ADC
 * 
 */
class AdcCalibration{
  public:
    void readFromEEPROM();
    void writeToEEPROM();

    int getVoltageMultiplier();
    int getVoltageOffset();
    int getCurrentMultiplier();
    int getCurrentOffset();

    void setVoltageMultiplier(int cal);
    void setVoltageOffset(int cal);
    void setCurrentMultiplier(int cal);
    void setCurrentOffset(int cal);

  private:
    int voltageMultiplier;       //measured voltage - calibration multiplier 0-255 (128)
    int voltageOffset;           //measured voltage - calibration offset     0-255 (100)
    int currentMultiplier;       //measured current - calibration multiplier 0-255 (71)
    int currentOffset;           //measured current - calibration offset     0-255 (127)
};

class DacCalibration{
  public:
    void readFromEEPROM();
    void writeToEEPROM();

    int getSetCurrentMultiplier();
    int getSetCurrentOffset();

    void setSetCurrentMultiplier(int cal);
    void setSetCurrentOffset(int cal);

  private:
    int setCurrentMultiplier;    //set current - calibration multiplier 0-255 (200)
    int setCurrentOffset;        //set current - calibration offset     0-255 (127)
};

#endif