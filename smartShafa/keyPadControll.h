#include KEYPAD_CONTROLL_H
#define KEYPAD_CONTROLL_H

#include <Keypad.h>
#include <config.h>
#include <pin.h>

class keyPadControll
{
private:
    char enteredPin[4];
    char currentPin;
    char tempPin;
    bool isCorrectPin;
    bool changePinMode;
    bool isEnteredPin;
    Keypad &isPressed;
    Storage &storage;

public:
    keyPadControll(Storage &storage);
    bool isCorrectPin();
    void changePin(CHANGE_PIN);
}

#endif