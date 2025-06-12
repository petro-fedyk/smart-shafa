#ifndef KEYPADCONTROL_H
#define KEYPADCONTROL_H

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "transistor.h"
#include "config.h"
#include "pin.h"
#include "storage.h"
#include "clock.h"
#include "buzzer.h"

class KeyPadControl
{
private:

    unsigned long messageStartTime;
    bool showingMessage;
    uint8_t pendingState;
    
    void handleMessageTimeout();

    uint8_t enteredPin[4];
    uint8_t pinIndex;
    LiquidCrystal_I2C &lcd;
    Transistor &transistor;
    bool changePasswordMode;
    uint8_t changePasswordStage;
    String tempPin;
    Storage &storage;
    String readedPin;
    MyClock &RTclock;
    unsigned long lastKeyPressTime = 0;
    String enterPin();
    myBuzzer &buzzer;

public:
    KeyPadControl(LiquidCrystal_I2C &lcd, Storage &storage, Transistor &transistor, MyClock &RTclock, myBuzzer &buzzer); // Оголошення конструктора з посиланням

    uint8_t lcdState = 0;

    void keyPadSetup();
    void keyPadLoop();
    bool isUnlockCodeCorrect();
    void clearPin();
    void lcdStateMachine(uint8_t &state);
    Keypad customKeypad;
    bool isKeyPressed;
    bool isKeyUnlock = false;
    bool isSuccess = false;
    bool tryToUnlock = false;
};

#endif