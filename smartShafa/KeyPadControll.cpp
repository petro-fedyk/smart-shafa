#include <Keypad.h>
#include "keyPadControll.h"

keyPadControll::keyPadControll(Storage &storage);
bool keyPadControll::isCorrectPin()
{
    char storedPin[4];
    storage.loadPassword(0, storedPin, 4);

    if (key)
    {
        isEnteredPin = true; // todo
        if (key == BTN_CONFIRM)
        {
            for (int i = 0; i < 4; i++)
            {
                if (enteredPin[i] != storedPin[i])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
    else
    {
        isEnteredPin = false;
    }
}

void keyPadControll::changePin()
{
    static bool waitingForCurrentPin = true;
    static bool waitingForNewPin = false;
    static bool waitingForConfirmNewPin = false;
    static char newPin[4];
    static int pinIndex = 0;

    if (key)
    {
        if (key == BTN_RESET)
        {
            memset(enteredPin, 0, 4);
            memset(newPin, 0, 4);
            pinIndex = 0;
            waitingForCurrentPin = true;
            waitingForNewPin = false;
            waitingForConfirmNewPin = false;
            return;
        }

        if (waitingForCurrentPin)
        {
            if (key == BTN_CONFIRM)
            {
                char storedPin[4];
                storage.loadPassword(0, storedPin, 4);

                if (memcmp(enteredPin, storedPin, 4) == 0)
                {
                    memset(enteredPin, 0, 4);
                    pinIndex = 0;
                    waitingForCurrentPin = false;
                    waitingForNewPin = true;
                }

                else
                {
                    if (pinIndex < 4)
                    {
                        enteredPin[pinIndex++] = key;
                    }
                }
            }
            else if (waitingForNewPin)
            {
                if (key == BTN_CONFIRM)
                {
                    memcpy(newPin, enteredPin, 4);
                    memset(enteredPin, 0, 4);
                    pinIndex = 0;
                    waitingForNewPin = false;
                    waitingForConfirmNewPin = true;
                }
                else
                {
                    if (pinIndex < 4)
                    {
                        enteredPin[pinIndex++] = key;
                    }
                }
            }
            else if (waitingForConfirmNewPin)
            {
                if (key == BTN_CONFIRM)
                {
                    if (memcmp(enteredPin, newPin, 4) == 0)
                    {
                        storage.savePassword(0, newPin, 4);
                    }

                    memset(enteredPin, 0, 4);
                    memset(newPin, 0, 4);
                    pinIndex = 0;
                    waitingForConfirmNewPin = false;
                }
                else
                {
                    if (pinIndex < 4)
                    {
                        enteredPin[pinIndex++] = key;
                    }
                }
            }
        }
    }
}