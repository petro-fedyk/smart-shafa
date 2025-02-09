#ifndef MAIN_CONTROLL_H
#define MAIN_CONTROLL_H

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "wifiConnect.h"
#include "keyPadControll.h"
#include "mainControll.h"
#include "clock.h"
#include "config.h"
#include "ota.h"
#include "pin.h"
#include "security.h"
#include "transistor.h"
#include "lcd.h"
#include "storage.h"

class MainControll
{
private:
    keyPadControll *isCorrectPin;
    keyPadControll *changePinMode;
    keyPadControll *isEnteredPin;
    Keypad &isPressed;
    Transistor *on();
    Transistor *off();
    MyLCD *printLcdData();
    clock *initClock();

public:
    void mainControll();
}

#endif
