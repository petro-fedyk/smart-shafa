#include KEYPAD_CONTROLL_H
#define KEYPAD_CONTROLL_H

#include <Keypad.h>
#include "transistor.h"
#include <config.h>
#include <pin.h>

extern char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS];

// ! todo
class KeyPadControll
{
private:
    char enteredPin[4] = {};
    char currentPin[4] = "1234";
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
    KeyPadControll(Storage &storage, LiquidCrystal_I2C &lcd);
    bool isCorrectPin();
    void changePin();
    void keyPadLoop();
}

#endif