#include KEYPAD_CONTROLL_H
#define KEYPAD_CONTROLL_H

#include <Keypad.h>
#include "transistor.h"
#include <config.h>
#include <pin.h>

extern char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS];

class keyPadControll
{
private:
    char enteredPin[4];
    char currentPin;
    bool isCorrectPin;
    bool changePinMode;
    bool isEnteredPin;
    uint8_t pinIndex;
    char key;
    Keypad &isPressed;
    Storage &storage;
    LiquidCrystal_I2C &lcd;
    Transistor &on();
    Transistor &off();
    char key = isPressed.getKey();

public:
    keyPadControll(Storage &storage, LiquidCrystal_I2C &lcd);
    bool isCorrectPin();
    void changePin();
    void keyPadLoop();
}

#endif