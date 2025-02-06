#include <Keypad.h>
#include "keyPadControll.h"

keyPadControll::keyPadControll();
bool keyPadControll::isCorrectPin()
{
    if (currentPin == enteredPin)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void keyPadControll::changePin()
{
    if (key)
    {
        if (key == CHANGE_PIN)
        {
            changePinMode = true;
        }
    }
}