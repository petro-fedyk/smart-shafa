#include "lcd.h"

LCD::LCD(uint8_t address, uint8_t columns, uint8_t rows)
    : lcd(address, columns, rows), columns(columns), rows(rows), backlightState(false)
{
}

void LCD::init()
{
    lcd.init();
    lcd.clear();
    backlight();
}

void LCD::clear()
{
    lcd.clear();
}

//? ARE ME NEEDED?
//void LCD::print(const String &message)
//{
//  lcd.print(message);
//}

void LCD::setCursor(uint8_t col, uint8_t row)
{
    if (col <= columns && row <= rows)
    {
        lcd.setCursor(col, row);
    }
}

void LCD::backlight()
{
    lcd.backlight();
    backlightState = true;
}

void LCD::noBacklight()
{
    lcd.noBacklight();
    backlightState = false;
}

void LCD::toggleBacklight()
{
    if (backlightState)
    {
        noBacklight();
    }
    else
    {
        backlight();
    }
}

bool LCD::getBacklightState()
{
    return backlightState;
}

void LCD::lcdStateMachine(uint8_t &state)
{
    switch (state)
    {
    case 0:
        lcd.clear();
        lcd.print("Enter Code:");
        break;
    case 1:
        lcd.clear();
        lcd.print("Enter Old Pass:");
        break;
    case 2:                    
        lcd.clear();
        lcd.print("Confirm New Pass:");

        break;
    case 3:
        lcd.clear();
        lcd.print("Enter New Pass:");

        break;
    case 4:
        lcd.clear();
        lcd.print("Pass Changed");

        break;
    case 5:
        lcd.clear();
        lcd.print("Mismatch");

        break;
    case 6:
        lcd.clear();
        lcd.print("Wrong Pass");
        break;
    case 7:
        lcd.clear();

//!CLOCK HERE
        lcd.print("Clock");
        break;
    }   
}