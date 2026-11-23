
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

#include "defines.hh"
#include "measurements.hh"
#include "controls.hh"
#include "user_input.hh"
#include "lcd_characters.hh"
#include "keypad_config.hh"
#include "encoder.hh"
#include "init.hh"
#include "mode_screen.hh"
#include "remote.hh"

int main(void){
/////////Arduino stuff//////////
  init();
  #if defined(USBCON)
    USBDevice.attach();
  #endif
////////////////////////////////

  Measurements measurements;
  Controls controls(measurements);
  UserInput userInput;
  LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4);
  Keypad keypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
  Encoder encoder;
  Battery battery;
  Transient transient;

  lcd.init();
  lcd.backlight();              //turn the backlight on
  lcd.createChar(degree, degreeSymbol);
  lcd.createChar(ohm, ohmSymbol);
  encoder.begin();

  Serial.begin(9600);

  pinInit();

  controls.loadOff(lcd);

  displayWelcomeScreen(lcd);
  delay(2000);
  displayMenu(lcd);

#ifdef SIM_TEST     //simulation build: skip the keypad menu, go straight to the serial remote mode
  Serial.println("READY");
  for(;;)
    remoteMode(lcd, userInput, keypad, measurements, controls);
#endif

  for (;;) {  //void loop() equivalent
		switch (keypad.getKey()){
			case '1':
				constCurrentMode(lcd, userInput, keypad, encoder, measurements, controls);
        displayMenu(lcd);
				break;
			case '2':
				constPowerMode(lcd, userInput, keypad, encoder, measurements, controls);
        displayMenu(lcd);
				break;
			case '3':
				constResistanceMode(lcd, userInput, keypad, encoder, measurements, controls);
        displayMenu(lcd);
				break;
			case '4':
				transientResponseMode(lcd, userInput, keypad, encoder, measurements, controls, transient);
        displayMenu(lcd);
				break;
			case '5':
				batteryCapacityMode(lcd, userInput, keypad, encoder, measurements, controls, battery);
        displayMenu(lcd);
				break;
			case '6':
				remoteMode(lcd, userInput, keypad, measurements, controls);
        displayMenu(lcd);
				break;
			// case '7':
			// 	calibration(lcd, userInput, keypad, encoder, measurements, controls);
      //  displayMenu(lcd);
			// 	break;
			default:
				delay(10);	//wait 10ms before checking again what keypad was pressed
				break;
		}

    ////////////Arduino stuff/////////////
	  if (serialEventRun) serialEventRun();
    //////////////////////////////////////
	}
  return 0;
}