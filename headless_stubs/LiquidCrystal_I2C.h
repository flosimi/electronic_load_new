// HEADLESS STUB - no display, every call is ignored. Only used by the "headless" PlatformIO environment.
#ifndef HEADLESS_LCD_STUB_H
#define HEADLESS_LCD_STUB_H
#include <Arduino.h>

class LiquidCrystal_I2C : public Print {
  public:
    LiquidCrystal_I2C(uint8_t, uint8_t, uint8_t){}
    void init(){}
    void backlight(){}
    void clear(){}
    void cursor(){}
    void noCursor(){}
    void setCursor(uint8_t, uint8_t){}
    void createChar(uint8_t, uint8_t*){}
    virtual size_t write(uint8_t){ return 1; }
    using Print::write;
};
#endif
