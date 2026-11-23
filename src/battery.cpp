
#include "battery.hh"

Battery::Battery(){
  cutoffVoltage.init(4, 2, 0, MAX_VOLTAGE, 0);
  dischargeCurrent.init(4, 2, 0.01, MAX_CURRENT, 0.01);
}

/**
 * @brief displays battery capacity in mAh, in the top left corner of the lcd
 * 
 * @param lcd 
 */
void Battery::displayCapacity(LiquidCrystal_I2C& lcd){
  char displayValue[20];
  dtostrf(capacity, 5, 0, displayValue);
  lcd.setCursor(0, 0);
	lcd.print(displayValue);
  lcd.print("mAh");
}