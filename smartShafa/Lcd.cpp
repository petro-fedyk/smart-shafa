#include <LiquidCrystal_I2C.h>
#include "lcd.h"

MyLCD::myLcd(*lcdData) : lcdData(lcdData), state(false)
{
    lcd.init();
    lcd.clear();
}
{
    this = > lcdData;
}
void MyLCD::printLcdData()
{
    lcd.clear();
    lcd.print(*lcdData);
}

void MyLCD::backLightOn()
{
    if (state)
    {
        lcd.backlight();
        state = true;
    }
}

void MyLCD::backLightOff()
{
    if (state)
    {
        lcd.noBacklight();
        state = false;
    }
}

void MyLCD::toggleBackLight()
{
    if (state)
    {
        backLightOff();
    }
    else
    {
        backLightOn();
    }
}





//currently not used
// void MyLCD::turnBackLight()
// {
    // backlightState = true;
    // should be implemented like we made in transistor    
    // this part must be in keypad handler from which toggle backlight is called
    // if (key)
    // {
    //     if (key == BACKLIGHT)
    //     {
    //         backlightState = !backlightState;
    //         if (backlightState)
    //         {
    //             backlight();
    //         }
    //         else
    //         {
    //             noBacklight();
    //         }
    //     }
    // }
// }