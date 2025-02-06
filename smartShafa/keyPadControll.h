#include KEYPAD_CONTROLL_H
#define KEYPAD_CONTROLL_H

#include <Keypad.h>
#include <config.h>
#include <pin.h>

class keyPadControll
{
private:
    uint8_t enteredPin[4];
    uint8_t currentPin;
    uint8_t tempPin;
    bool isCorrectPin;
    bool changePinMode;
    Keypad &isPressed;

public:
    keyPadControll();
    bool isCorrectPin();
    void changePin(CHANGE_PIN);
}

#endif