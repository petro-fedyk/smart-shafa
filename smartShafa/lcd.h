#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "keyPadControll.h"

class MyLCD
{
private:
    LiquidCrystal_I2C *lcdData;

    bool state;

public:
    uint8_t lcdState = 0;
    bool backlightState;
    MyLCD(LiquidCrystal_I2C *lcdData);
    void printLcdData();
    void backLightOn();
    void backLightOff();
    void toggleBackLight();
    void lcdMode(lcdState);
}

#endif