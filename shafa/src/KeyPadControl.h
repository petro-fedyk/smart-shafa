#ifndef KEYPADCONTROL_H
#define KEYPADCONTROL_H

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "transistor.h"
#include "config.h"
#include "pin.h"
#include "storage.h"
#include "clock.h"

class KeyPadControl
{
private:
    uint8_t enteredPin[4];
    uint8_t pinIndex;
    LiquidCrystal_I2C &lcd;
    Transistor &transistor;
    bool changePasswordMode;
    uint8_t changePasswordStage;
    String tempPin;
    Storage &storage;
    String readedPin;
    MyClock &clock;
    unsigned long lastKeyPressTime = 0;

public:
    KeyPadControl(LiquidCrystal_I2C &lcd, Storage &storage, Transistor &transistor, MyClock &clock); // Оголошення конструктора з посиланням

    void keyPadSetup();
    void keyPadLoop();
    bool isUnlockCodeCorrect();
    void clearPin();
    Keypad customKeypad;
    bool backlight = true;
    bool isKeyPressed;
};

#endif