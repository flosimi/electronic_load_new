
#include "set_value.hh"


void SetValue::init(int digits, int decimals, float min, float max, float init_value){
  numOfDigits = digits;
  numOfDecimalPlaces = decimals;
  minValue = min;
  maxValue = max;
  maxDecimalPlace = static_cast<DecimalPlaces> (digits - decimals -1);
  minDecimalPlace = static_cast<DecimalPlaces> (-decimals);
  value = init_value;
}

void SetValue::display(LiquidCrystal_I2C& lcd){
  char displayValue[20];
  int decimal = 0;
  if(numOfDecimalPlaces > 0)  //if value has decimal point
    decimal = 1;
  dtostrf(value, (numOfDigits + decimal), numOfDecimalPlaces, displayValue);
	lcd.print(displayValue);
}


void SetValue::limit(){
  if(value < minValue)
    value = minValue;
  else if(value > maxValue)
    value = maxValue;
}


void operator--(DecimalPlaces& dec){
  int tmp = static_cast<int>(dec);
  --tmp;
  dec = static_cast<DecimalPlaces>(tmp);
}