

#include "user_input.hh"

UserInput::UserInput(){
  setCurrent.init(4, 2, 0.01, MAX_CURRENT, 0.1);
  setPower.init(5,1, 0.1, MAX_POWER, 1);
  setResistance.init(5,1, 0.1, MAX_RESISTANCE, 100);
}

void UserInput::inputFromKeypad(LiquidCrystal_I2C& lcd, SetValue& setParameter, int xPositon, int yPosition){
	if(key >= '0' && key <= '9'){	//is digit
		if(index <=5){	//limit number of input characters
			numbers[index] = key;
			numbers[++index] = '\0';
			lcd.setCursor(x_pos,3);                              
			lcd.print(key);  //show input number on the LCD
			x_pos++;
			time = millis();
		}
	}

	else if(key == '.' && !decimalPointPresent){
		if(index <=5){	//limit number of input characters
			numbers[index] = '.';
			numbers[++index] = '\0';
			lcd.setCursor(x_pos,3);
			lcd.print(".");
			x_pos++;
			decimalPointPresent = true;
			time = millis();
		}
	}
	
	else if(key == Delete && index > 0){	//nothing to delete when the input is empty (would write numbers[-1])
		lcd.setCursor(--x_pos,3);
		lcd.print(" ");
		--index;
		if(numbers[index] == '.')		//deleted decimal point
			decimalPointPresent = false;
		numbers[index] = '\0';
		time = millis();
	}

	else if(key == Enter && index > 0) {
		setParameter.value = atof(numbers);		//convert array of entered numbers to float and write it to SetValue variable
		setParameter.limit();
		lcd.setCursor(xPositon, yPosition);
		setParameter.display(lcd);
		lcd.setCursor(0,3);
		lcd.print("       ");		//clear all entered numbers from the screen
    resetKeypadInput();
	}
}

void UserInput::resetKeypadInput(){
  for(int i = 0; i < 7; ++i)
    numbers[i] = '\0';
  decimalPointPresent = false;
  index = 0;
  x_pos = 0;
}


void UserInput::checkEncoder(LiquidCrystal_I2C& lcd, SetValue& setParameter, Encoder& encoder, int xPositon, int yPosition){
	if(encoder.wasButtonPressed()){
		time = millis();
		if(decimalPlace > setParameter.minDecimalPlace){	//if did't reach the last digit of setParameter
			--decimalPlace;																	//move cursor to the left
			++cursorPosX;
		}   
		else{
			decimalPlace = setParameter.maxDecimalPlace;		//move the cursor back to the first digit of setParameter
			cursorPosX = xPositon;
		}
		if(decimalPlace == tenths)	//jump across the decimal point
			++cursorPosX;
	}

	int steps = encoder.takeRotation();
	if(steps){
		setParameter.value += steps * pow(10, decimalPlace);
		setParameter.limit();
		lcd.setCursor(xPositon, yPosition);
		setParameter.display(lcd);
		time = millis();
	}
}