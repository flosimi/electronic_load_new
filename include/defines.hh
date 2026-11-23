
#ifndef DEFINES_HH
#define DEFINES_HH

#define MAX_CURRENT 8.4
#define MAX_POWER 400
#define MAX_VOLTAGE 50
#define MAX_RESISTANCE 9999.9

//////////////////////////////////////////////////////////////////////////////
// CURRENT PATH HARDWARE (8 A main board: 8 x 0.2 ohm shunts, 4 parallel branches)
//////////////////////////////////////////////////////////////////////////////
#define CURRENT_SHUNT_OHM 0.025       //equivalent resistance of all shunts in parallel [ohm]
#define ADC_CURRENT_FULL_SCALE 0.256  //ADS1115 range used for the shunt voltage (GAIN_SIXTEEN = +/-0.256V)
#define DAC_VREF 4.096                //MCP4725 reference voltage [V]
#define DAC_ATTENUATION 0.053         //divider between DAC output and the regulation loop (0.053 in the original design)

//DAC volts needed per 1A of load current
#define DAC_VOLTS_PER_AMP (CURRENT_SHUNT_OHM / DAC_ATTENUATION)

//compile-time sanity checks of the hardware settings above
static_assert(MAX_CURRENT * CURRENT_SHUNT_OHM < ADC_CURRENT_FULL_SCALE, "Shunt voltage at MAX_CURRENT exceeds ADC range");
static_assert(MAX_CURRENT * DAC_VOLTS_PER_AMP < DAC_VREF, "DAC can't reach MAX_CURRENT");

#define ADC_ADDRESS 0x48
#define DAC_ADDRESS 0x60
#define LCD_ADDRESS 0x27
#define RTC_ADDRESS 0x6f

#define EEPROM_ADDRESS_DAC_MULTIPLIER_CAL 0       //address for calibration value of DAC - setCurrentMultiplier
#define EEPROM_ADDRESS_DAC_OFFSET_CAL 1           //address for calibration value of DAC - setCurrentOffset
#define EEPROM_ADDRESS_ADC_U_MULTIPLIER_CAL 2     //address for calibration value of ADC - voltageMultiplier
#define EEPROM_ADDRESS_ADC_U_OFFSET_CAL 5         //address for calibration value of ADC - voltageOffset
#define EEPROM_ADDRESS_ADC_I_MULTIPLIER_CAL 3     //address for calibration value of ADC - currentMultiplier
#define EEPROM_ADDRESS_ADC_I_OFFSET_CAL 4         //address for calibration value of ADC - currentOffset

#define KEYPAD_ROWS 4   //number of rows of the keypad
#define KEYPAD_COLS 4   //number of collumns of the keypad
#define KP1 6           //Keypad pin
#define KP2 7           //Keypad pin
#define KP3 8           //Keypad pin
#define KP4 9           //Keypad pin
#define KP5 10          //Keypad pin
#define KP6 14          //Keypad pin
#define KP7 15          //Keypad pin
#define KP8 16          //Keypad pin
#define KP9 11          //Keypad pin

#define ENCODER_A 2
#define ENCODER_B 17
#define ENCODER_BUTTON 3

#define THERMISTOR A6
#define R1 10000
#define c1 1.009249522e-03
#define c2 2.378405444e-04
#define c3 2.019202697e-07

#define FAN 5
#define OUTPUT_OFF 4
#define IN_TRIGGER A7   //analog-only pin (ADC7)

#endif