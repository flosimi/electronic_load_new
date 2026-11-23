
#include "measurements.hh"

Measurements::Measurements(){
  adc.begin(ADC_ADDRESS);
  adc.setDataRate(RATE_ADS1115_860SPS);
  calibration.readFromEEPROM();
}

void Measurements::update(){
  fault = 0;
  measureVoltage();
  measureCurrent();
  measureTemperature();
  if(voltage > OVERVOLTAGE_TRIP)
    fault |= FaultOverVoltage;
  if(temperature > MAX_TEMPERATURE)
    fault |= FaultOverTemp;
  power = voltage * current;
}

//one ADS1115 single-shot conversion; gives up after ADC_TIMEOUT_MS instead of waiting forever like readADC_*()
bool Measurements::readAdc(uint16_t mux, int16_t& result){
  adc.startADCReading(mux, false);
  uint32_t start = millis();
  while(!adc.conversionComplete()){
    if(millis() - start > ADC_TIMEOUT_MS){
      fault |= FaultAdc;
      return false;
    }
  }
  result = adc.getLastConversionResults();
  return true;
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
  int16_t raw;
  if(!readAdc(ADS1X15_REG_CONFIG_MUX_DIFF_2_3, raw))
    return voltage;     //keep the last value, fault is set
  voltage = ((raw / 32768.0) * 0.512 * 100) * (0.95 + calibration.getVoltageMultiplier()/2550.0) + (-0.005 + calibration.getVoltageOffset()/25500.0);
  noLessThanZero(voltage);
  return voltage;
}


float Measurements::measureCurrent(){
  adc.setGain(GAIN_SIXTEEN);  // 16x gain  +/- 0.256V  1 bit = 0.0078125mV
            //      reading_from_adc / resolution * Vmax / R(all shunts in parallel) * (0.95 to 1.05) + (0 to 0.01)
  int16_t raw;
  if(!readAdc(ADS1X15_REG_CONFIG_MUX_SINGLE_0, raw))
    return current;     //keep the last value, fault is set
  current = (((raw / 32768.0) * ADC_CURRENT_FULL_SCALE / CURRENT_SHUNT_OHM)) * (0.95 + calibration.getCurrentMultiplier()/2550.0) + calibration.getCurrentOffset()/25500.0;
  noLessThanZero(current);
  return current;
}


int Measurements::measureTemperature(){
  int Vo;
  float R2, logR2;
  Vo = analogRead(THERMISTOR);
#ifndef SIM_TEST    //the simulation has no thermistor connected
  if(Vo < THERMISTOR_ADC_MIN || Vo > THERMISTOR_ADC_MAX){  //thermistor open / shorted: log(0) or 1/0 would read -273 deg C and turn the fan off
    fault |= FaultThermistor;
    temperature = MAX_TEMPERATURE;                          //above the fan's full-speed threshold
    return temperature;
  }
#endif
  R2 =R1 * (1023.0 / (float)Vo - 1.0);
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