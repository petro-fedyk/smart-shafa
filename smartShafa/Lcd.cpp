#include <LiquidCrystal_I2C.h>
#include "lcd.h"

MyLCD::myLcd(*lcdData) : lcdData(lcdData), state(false)
{
    this = > lcdData;
}
void MyLcd::lcdSetup()
{
    lcd.init();
    lcd.clear();
}
void MyLCD::printLcdData()
{
    lcd.clear();
    lcd.print(*lcdData);
}

void MyLCD::backLightOn()
{
    if (!state)
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

void lcdMode(lcdState)
{
    switch (lcdState)
    {
    case 0:
        lcd.clear();
        lcd.print("Enter PIN:");
        break;
    case 1:
        lcd.clear();
        lcd.print("Enter New PIN:");
        break;
    case 2:
        lcd.clear();
        lcd.print("Confirm New PIN:");
        break;
    case 3:
        lcd.clear();
        lcd.print("Incorrect PIN!");
        break;
    case 4:
        lcd.clear();
        lcd.print("Access Granted");
        break;
    case 5:
        lcd.clear();
        lcd.print("Wrong PIN");
        break;
    default:
        break;
    }
}

// currently not used
//  void MyLCD::turnBackLight()
//  {
//  backlightState = true;
//  should be implemented like we made in transistor
//  this part must be in keypad handler from which toggle backlight is called
//  if (key)
//  {
//      if (key == BACKLIGHT)
//      {
//          backlightState = !backlightState;
//          if (backlightState)
//          {
//              backlight();
//          }
//          else
//          {
//              noBacklight();
//          }
//      }
//  }
// }