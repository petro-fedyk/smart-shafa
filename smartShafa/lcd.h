#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "keyPadControll.h"

class MyLCD
{
private:
    LiquidCrystal_I2C *lcdData;

public:
    myLcd(LiquidCrystal_I2C *lcdData);
    bool lcdState;
    bool backlightState;
    void printLcdData();
    void backLightOn();
    void backLightOff();
    void toggleBackLight();
    void lcdMode(lcdState);
    void showUnlockData();
    void showClockAndWeather();
    void lcdSetup();
}

#endif