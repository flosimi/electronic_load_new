
#include "remote.hh"
#include "mode_screen.hh"

namespace {

const uint8_t BUFFER_SIZE = 24;

char buffer[BUFFER_SIZE];
uint8_t length = 0;
bool overflow = false;
ModeOfOperation mode = ConstCurrent;

const char* modeName(){
	switch(mode){
		case ConstPower:      return "CP";
		case ConstResistance: return "CR";
		default:              return "CC";
	}
}

SetValue& setpoint(UserInput& userInput){
	switch(mode){
		case ConstPower:      return userInput.setPower;
		case ConstResistance: return userInput.setResistance;
		default:              return userInput.setCurrent;
	}
}

#ifndef HEADLESS	//no display in the headless build
void showHeader(LiquidCrystal_I2C& lcd, Controls& controls){
	lcd.setCursor(0, 0);
	lcd.print("PC Remote ");
	lcd.print(modeName());
	lcd.print("     ");
	lcd.setCursor(17, 0);
	lcd.print(controls.isLoadOn() ? "ON " : "OFF");
}

void showSetpoint(LiquidCrystal_I2C& lcd, UserInput& userInput){
	lcd.setCursor(0, 2);
	lcd.print("                    ");
	lcd.setCursor(0, 2);
	switch(mode){
		case ConstPower:
			lcd.print("Set P=");
			userInput.setPower.display(lcd);
			lcd.print("W");
			break;
		case ConstResistance:
			lcd.print("Set R=");
			userInput.setResistance.display(lcd);
			lcd.write(ohm);
			break;
		default:
			lcd.print("Set I=");
			userInput.setCurrent.display(lcd);
			lcd.print("A");
			break;
	}
}
#endif

void sendMeasurements(UserInput& userInput, Measurements& measurements, Controls& controls){
	bool on = controls.isLoadOn();
	Serial.print("M;");
	Serial.print(measurements.voltage, 3);
	Serial.print(";");
	Serial.print(on ? measurements.current : 0.0f, 3);
	Serial.print(";");
	Serial.print(on ? measurements.power : 0.0f, 2);
	Serial.print(";");
	Serial.print(measurements.temperature);
	Serial.print(";");
	Serial.print(on ? 1 : 0);
	Serial.print(";");
	Serial.print(modeName());
	Serial.print(";");
	Serial.println(setpoint(userInput).value, 3);
}

void replyError(){
	Serial.println("ERR");
}

bool execute(char* command, LiquidCrystal_I2C& lcd, UserInput& userInput, Measurements& measurements, Controls& controls){
	for(char* c = command; *c; ++c)
		*c = toupper(*c);

	char* argument = strchr(command, ' ');
	if(argument){
		*argument = '\0';
		++argument;
	}

	ModeOfOperation newMode = mode;
	if(!strcmp(command, "CC"))
		newMode = ConstCurrent;
	else if(!strcmp(command, "CP"))
		newMode = ConstPower;
	else if(!strcmp(command, "CR"))
		newMode = ConstResistance;
	else if(!strcmp(command, "ON")){
		if(measurements.fault){	//sensor failure / over-voltage / over-temperature
			replyError();
			return false;
		}
		controls.loadOn(lcd);
		Serial.println("OK");
		return false;
	}
	else if(!strcmp(command, "OFF")){
		controls.loadOff(lcd);
		Serial.println("OK");
		return false;
	}
	else if(!strcmp(command, "MEAS?")){
		sendMeasurements(userInput, measurements, controls);
		return false;
	}
	else if(!strcmp(command, "HELP")){
		Serial.println("CC <A> | CP <W> | CR <ohm> | ON | OFF | MEAS? | HELP | EXIT");
		return false;
	}
	else if(!strcmp(command, "EXIT")){
		controls.loadOff(lcd);
		Serial.println("OK");
		return true;
	}
	else{
		replyError();
		return false;
	}

	//CC / CP / CR <value>
	if(!argument || (!isdigit(argument[0]) && argument[0] != '.')){
		replyError();
		return false;
	}
	mode = newMode;
	SetValue& value = setpoint(userInput);
	value.value = atof(argument);
	value.limit();
#ifndef HEADLESS
	showHeader(lcd, controls);
	showSetpoint(lcd, userInput);
#endif
	Serial.print("OK ");
	Serial.print(modeName());
	Serial.print(" ");
	Serial.println(value.value, 3);
	return false;
}

bool pollSerial(LiquidCrystal_I2C& lcd, UserInput& userInput, Measurements& measurements, Controls& controls){
	while(Serial.available() > 0){
		char c = Serial.read();
		if(c == '\r')
			continue;
		if(c == '\n'){
			bool wasOverflow = overflow;
			buffer[length] = '\0';
			length = 0;
			overflow = false;
			if(wasOverflow){
				replyError();
				continue;
			}
			if(buffer[0] == '\0')
				continue;
			if(execute(buffer, lcd, userInput, measurements, controls))
				return true;
		}
		else if(length < BUFFER_SIZE - 1)
			buffer[length++] = c;
		else
			overflow = true;
	}
	return false;
}

} //namespace

int remoteMode(LiquidCrystal_I2C& lcd, UserInput& userInput, Keypad& keypad, Encoder& encoder, Measurements& measurements, Controls& controls){
	length = 0;
	overflow = false;
	mode = ConstCurrent;
	controls.loadOff(lcd);

#ifndef HEADLESS
	lcd.clear();
	showHeader(lcd, controls);
	showSetpoint(lcd, userInput);
	lcd.setCursor(0, 3);
	lcd.print("Menu = exit");
#endif

	while(Serial.available() > 0)	//drop old data
		Serial.read();

	encoder.reset();
	uint8_t reportedFault = 0;

	while(1){
		measurements.update();
#ifndef HEADLESS
		measurements.displayMeasurements(lcd, controls.isLoadOn());
#endif
		controls.fanControll();

		//messages that don't answer a command start with '!' so that a host waiting for "OK"/"ERR" can tell them apart
		if(encoder.wasButtonPressed() && controls.isLoadOn()){	//physical safety switch
			controls.loadOff(lcd);
			Serial.println("!OFF ENCODER");
		}

		if(measurements.fault != reportedFault){	//report each change of the protection flags once
			reportedFault = measurements.fault;
			if(reportedFault){
				controls.loadOff(lcd);
				Serial.print("!FAULT ");
				Serial.println(reportedFault);
			}
		}

		switch(mode){
			case ConstPower:      controls.regulatePower(userInput.setPower.value); break;
			case ConstResistance: controls.regulateResistance(userInput.setResistance.value); break;
			default:              controls.regulateCurrent(userInput.setCurrent.value); break;
		}

		if(pollSerial(lcd, userInput, measurements, controls))
			return 0;

#ifndef HEADLESS
		switch(keypad.getKey()){
			case Menu:
				controls.loadOff(lcd);
				return 0;
			case LoadOnOff:
				controls.loadOnOffToggle(lcd);
				break;
			default:
				break;
		}
#endif
	}
}
