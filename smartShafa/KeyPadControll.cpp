#include <Keypad.h>
#include "KeyPadControll.h"

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'E', '0', 'F', 'D'}};

uint8_t rowPins[KEYPAD_ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};
uint8_t colPins[KEYPAD_COLS] = {PIN_COL_1, PIN_COL_2, PIN_COL_3, PIN_COL_4};

KeyPadControll::KeyPadControll(Storage &storage, LiquidCrystal_I2C &lcd);

bool KeyPadControll::isCorrectPin()
{
    uint8_t storedPin[4];
    int passwordCount = storage.getPasswordCount();

    for (int i = 0; i < passwordCount; i++)
    {
        storage.loadPassword(i, storedPin, 4);
        bool match = true;
        for (int j = 0; j < 4; j++)
        {
            if (enteredPin[j] != storedPin[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            return true;
        }
    }
    return false;
}

void KeyPadControll::keyPadLoop() // to do
{

    key = isPressed.getKey();

    if (key == BTN_CONFIRM)
    {
        if (isCorrectPin())
        {
            lcd.clear();
            lcd.print("Access Granted");
            Transistor.on();
            lockTime = millis();
            while (millis() - lockTime < 2000)
            {
            }
        }
        else
        {
            lcd.clear();
            lcd.print("Wrong Code");
            Transistor.off();
            lockTime = millis();
            while (millis() - lockTime < 2000)
            {
            }
        }
    }
    if (key == CHANGE_PIN)
    {
        changePin();
    }
    lcd.clear();
}

void KeyPadControll::changePin()
{
    static bool waitingForOldPin = true;
    static bool waitingForNewPin = false;
    static bool waitingForConfirmNewPin = false;
    static char newPin[4];
    static int pinIndex = 0;

    if (key == BTN_RESET)
    {
        memset(enteredPin, 0, 4);
        memset(newPin, 0, 4);
        pinIndex = 0;
        waitingForOldPin = true;
        waitingForNewPin = false;
        waitingForConfirmNewPin = false;
        lcd.clear();
        return;
    }

    if (key == CHANGE_PIN)
    {
        changePinMode = true;
        lcd.clear();
        lcd.print("Enter Old Pass:");
        pinIndex = 0;
        memset(enteredPin, 0, 4);
        return;
    }

    if (changePinMode)
    {
        if (key && key != BTN_CONFIRM)
        {
            if (pinIndex < 4)
            {
                enteredPin[pinIndex++] = key;
            }
        }

        if (key == BTN_CONFIRM)
        {
            if (waitingForOldPin)
            {
                if (isCorrectPin())
                {
                    lcd.clear();
                    lcd.print("Enter New PIN:");
                    pinIndex = 0;
                    memset(enteredPin, 0, 4);
                    waitingForOldPin = false;
                    waitingForNewPin = true;
                }
                else
                {
                    lcd.clear();
                    lcd.print("Incorrect PIN!");
                    pinIndex = 0;
                    memset(enteredPin, 0, 4);
                }
            }
            else if (waitingForNewPin)
            {
                memcpy(newPin, enteredPin, 4);
                lcd.clear();
                lcd.print("Confirm New PIN:");
                pinIndex = 0;
                memset(enteredPin, 0, 4);
                waitingForNewPin = false;
                waitingForConfirmNewPin = true;
            }
            else if (waitingForConfirmNewPin)
            {
                if (memcmp(enteredPin, newPin, 4) == 0)
                {
                    storage.savePassword(0, newPin, 4);
                    lcd.clear();
                    lcd.print("PIN Changed!");
                }
                else
                {
                    lcd.clear();
                    lcd.print("Pins Don't Match!");
                }
                pinIndex = 0;
                memset(enteredPin, 0, 4);
                memset(newPin, 0, 4);
                waitingForConfirmNewPin = false;
                changePinMode = false;
            }
        }
    }
}
