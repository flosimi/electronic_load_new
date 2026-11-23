
#include "measurements.hh"

Measurements::Measurements(){
  adc.begin(ADC_ADDRESS);
  calibration.readFromEEPROM();
}

void Measurements::update(){
  measureVoltage();
  measureCurrent();
  measureTemperature();
  power = voltage * current;
}

void Measurements::displayMeasurements(LiquidCrystal_I2C& lcd, bool loadIsOn){
  if(!loadIsOn){
    current = 0;
    power = 0;
  }
  lcd.setCursor(0,1);
  display(lcd, voltage, 4, 3);
  lcd.print("V ");
  display(lcd, current, 4, 3);
  lcd.print("A ");
  display(lcd, power, 4, 2);
  lcd.print("W");
  lcd.setCursor(16,3);
	lcd.print(temperature);
	lcd.write(degree);
	lcd.print("C");
}


float Measurements::measureVoltage(){
  adc.setGain(GAIN_EIGHT);  // 8x gain   +/- 0.512V  1 bit = 0.015625mV
            //        reading_from_adc / resolution * Vmax * attenuation * (0.95 to 1.05) + (-0.005 to 0.005)
  voltage = ((adc.readADC_Differential_2_3()/ 32768.0) * 0.512 * 100) * (0.95 + calibration.getVoltageMultiplier()/2550.0) + (-0.005 + calibration.getVoltageOffset()/25500.0);
  noLessThanZero(voltage);
  return voltage;
}


float Measurements::measureCurrent(){
  adc.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
            //      reading_from_adc / resolution * Vmax / R(all shunts in parallel) * (0.95 to 1.05) + (0 to 0.01)
  current = (((adc.readADC_SingleEnded(0) / 32768.0) * ADC_CURRENT_FULL_SCALE / CURRENT_SHUNT_OHM)) * (0.95 + calibration.getCurrentMultiplier()/2550.0) + calibration.getCurrentOffset()/25500.0;
  noLessThanZero(current);
  return current;
}


int Measurements::measureTemperature(){
  int Vo;
  float R2, logR2;
  Vo = analogRead(THERMISTOR);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  temperature = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
  temperature -= 273.15;
  return temperature;
}


void Measurements::display(LiquidCrystal_I2C& lcd, float value, int numOfDigits, int numOfDecimalPlaces){
  char displayValue[10];

   //when value is bigger than expected decrease the resolution to fit on the display 
  while(numOfDecimalPlaces > 0 && value >= pow(10, (numOfDigits-numOfDecimalPlaces)))
    --numOfDecimalPlaces;

  dtostrf(value, (numOfDigits + 1), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}


void Measurements::noLessThanZero(float& value){
  if(value < 0)
    value = 0;
}