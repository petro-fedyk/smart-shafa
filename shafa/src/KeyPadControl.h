#ifndef KEYPADCONTROL_H
#define KEYPADCONTROL_H

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "pin.h"

class KeyPadControl
{
private:
    uint8_t enteredPin[4];
    uint8_t pinIndex;
    LiquidCrystal_I2C &lcd;

    bool changePasswordMode;
    uint8_t changePasswordStage;
    uint8_t tempPin[4];

public:
    KeyPadControl(LiquidCrystal_I2C &lcd); // Оголошення конструктора з посиланням
    void keyPadSetup();
    void keyPadLoop();
    bool isUnlockCodeCorrect();
    void clearPin();
    Keypad customKeypad;
    bool isKeyPressed; // Просто оголошення змінної
};

#endif