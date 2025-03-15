#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "KeyPadControll.h"

class MyLCD
{
private:
    LiquidCrystal_I2C *lcdData;

    bool state;

public:
    void lcdSetup();
    bool backlightState;
    MyLCD(LiquidCrystal_I2C *lcdData);
    void printLcdData();
    void backLightOn();
    void backLightOff();
    void toggleBackLight();
}

#endif