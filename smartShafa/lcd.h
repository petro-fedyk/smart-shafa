#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "keyPadControll.h"

class MyLCD
{
private:
    LiquidCrystal_I2C *lcdData;
    bool backlightState;

public:
    MyLCD(LiquidCrystal_I2C *lcdData);
    void printLcdData();
    void turnBackLight();
}

#endif