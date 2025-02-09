#include <LiquidCrystal_I2C.h>
#include "lcd.h"

MyLCD::myLcd(*lcdData)
{
    this = > lcdData;
}
void MyLCD::printLcdData()
{
    lcd.clear();
    lcd.print(*lcdData);
}

void MyLCD::turnBackLight()
{
    backlightState = true;

    if (key)
    {
        if (key == BACKLIGHT)
        {
            backlightState = !backlightState;
            if (backlightState)
            {
                backlight();
            }
            else
            {
                noBacklight();
            }
        }
    }
}