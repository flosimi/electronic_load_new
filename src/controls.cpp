
#include "controls.hh"

Controls::Controls(Measurements& measurements_): measurements(measurements_){
	dac.begin(DAC_ADDRESS);
	calibration.readFromEEPROM();
}


void Controls::fanControll(){
	fanspeed = map(measurements.temperature, lowTemperatureThreshold, 45, 60, 255);
	//hysteresis, so that fans don't turn on/off repeatedly when crossing the threshold
  if (measurements.temperature < lowTemperatureThreshold){
    fanspeed = 0;
    lowTemperatureThreshold = 31;
  }
  if (measurements.temperature >= lowTemperatureThreshold)
     lowTemperatureThreshold=30;

  //if it's hot drive fans at full speed
  if (measurements.temperature > 45){
    fanspeed = 255;
	}
	analogWrite(FAN, fanspeed);
}

void Controls::loadOn(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, HIGH);
	lcd.setCursor(17, 0);
	lcd.print("ON ");
	loadIsOn = true;
}


void Controls::loadOff(LiquidCrystal_I2C& lcd){
	digitalWrite(OUTPUT_OFF, LOW);
	lcd.setCursor(17, 0);
	lcd.print("OFF");
	loadIsOn = false;
}


void Controls::loadOnOffToggle(LiquidCrystal_I2C& lcd){
	if(loadIsOn){
		loadOff(lcd);
	}
	else if(!loadIsOn){
		loadOn(lcd);
	}	
}

void Controls::regulateCurrent(float setCurrent){
	if(isnan(setCurrent))	//e.g. 0W / 0V in constant power mode
		setCurrent = 0;
	if(setCurrent > MAX_CURRENT)
		setCurrent = MAX_CURRENT;
	if(setCurrent * measurements.voltage > MAX_POWER)
		setCurrent = MAX_POWER / measurements.voltage;

								//setCurrent*resolution/Vref*equivalent R of shunts/attenuation * calibrationMultiplier + calibrationOffset
	int dacControl = setCurrent * 4096 / DAC_VREF * DAC_VOLTS_PER_AMP * (0.95 + calibration.getSetCurrentMultiplier()/2550.0) + (calibration.getSetCurrentOffset() - 128);
	if(dacControl < 0)						//////////////////////////////
		dacControl = 0;							//	ensure that dacControl	//
	else if(dacControl > 4095)		//	is between 0 and 4095		//
		dacControl = 4095;					//////////////////////////////
	dac.setVoltage(dacControl, false);
}

void Controls::regulatePower(float setPower){
	regulateCurrent(setPower/measurements.voltage);
}

void Controls::regulateResistance(float setResistance){
	regulateCurrent(measurements.voltage/setResistance);
}

bool Controls::isLoadOn(){
	return loadIsOn;
}