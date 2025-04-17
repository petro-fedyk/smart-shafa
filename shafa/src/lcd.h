#ifndef LCD_H
#define LCD_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCD
{
public:
    LCD(uint8_t address, uint8_t columns, uint8_t rows);
    void init();
    void clear();
  //?  void print(const String &message);
    void setCursor(uint8_t col, uint8_t row);
    void backlight();
    void noBacklight();
    void toggleBacklight();
    void lcdStateMachine(uint8_t &state);
    bool getBacklightState();

    uint8_t lcdstate = 0;

private:
    LiquidCrystal_I2C lcd;
    uint8_t columns;
    uint8_t rows;
    bool backlightState;
};
#endif