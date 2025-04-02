#include "KeyPadControl.h"
#include "config.h"

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'E', '0', 'F', 'D'}};

const String initialPassword = "1234";

uint8_t rowPins[KEYPAD_ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};
uint8_t colPins[KEYPAD_COLS] = {PIN_COL_1, PIN_COL_2, PIN_COL_3, PIN_COL_4};

KeyPadControl::KeyPadControl(LiquidCrystal_I2C &lcd)
    : customKeypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS), lcd(lcd), pinIndex(0), changePasswordMode(false), changePasswordStage(0)
{
  clearPin();
}

void KeyPadControl::keyPadSetup()
{
  lcd.init();
  lcd.backlight();
  clearPin();
  Serial.println("KeyPad Setup complete");
}

void KeyPadControl::keyPadLoop()
{
  char key = customKeypad.getKey();
  isKeyPressed = (key != NO_KEY); // Оновлення змінної isKeyPressed

  if (key)
  {
    if (key == CHANGE_PIN)
    {
      changePasswordMode = true;
      lcd.clear();
      lcd.print("Enter Old Pass:");
      pinIndex = 0;
      return;
    }

    if (changePasswordMode)
    {
      if (key == BTN_CONFIRM)
      {
        if (pinIndex == PASSWORD_LENGTH && isUnlockCodeCorrect())
        {
          lcd.clear();
          lcd.print("Enter New Pass:");
          pinIndex = 0;
          changePasswordStage = 1;
        }
        else if (changePasswordStage == 1 && pinIndex == PASSWORD_LENGTH)
        {
          memcpy(tempPin, enteredPin, PASSWORD_LENGTH);
          lcd.clear();
          lcd.print("Confirm New Pass:");
          pinIndex = 0;
          changePasswordStage = 2;
        }
        else
        {
          lcd.clear();
          lcd.print("Wrong Pass");
          clearPin();
          changePasswordMode = false;
        }
      }
      else if (key == BTN_RESET)
      {
        clearPin();
      }
      else
      {
        if (pinIndex < sizeof(enteredPin) / sizeof(enteredPin[0]))
        {
          enteredPin[pinIndex++] = key;

          String pinDisplay = "";
          for (int i = 0; i < pinIndex; i++)
          {
            pinDisplay += char(enteredPin[i]);
          }

          lcd.setCursor(0, 1);
          lcd.print(pinDisplay);
        }
      }
      return;
    }

    if (key == BTN_CONFIRM)
    {
      if (isUnlockCodeCorrect())
      {
        lcd.clear();
        lcd.print("Access Granted");
      }
      else
      {
        lcd.clear();
        lcd.print("Wrong Code");
      }
      clearPin();
    }
    else if (key == BTN_RESET)
    {
      clearPin();
    }
    else
    {
      if (pinIndex < sizeof(enteredPin) / sizeof(enteredPin[0]))
      {
        enteredPin[pinIndex++] = key;

        String pinDisplay = "";
        for (int i = 0; i < pinIndex; i++)
        {
          pinDisplay += char(enteredPin[i]);
        }

        lcd.setCursor(0, 1);
        lcd.print(pinDisplay);
      }
    }
  }
}

bool KeyPadControl::isUnlockCodeCorrect()
{
  uint8_t storedPin[4];
  bool match = true;

  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    for (int j = 0; j < PASSWORD_LENGTH; j++)
    {
      if (enteredPin[i] != initialPassword[j])
      {
        match = false;
        Serial.println("false pin");
        break;
      }
    }
    if (match)
    {
      return true;
      Serial.println("true pin");
    }
  }
  return false;
}

void KeyPadControl::clearPin()
{
  memset(enteredPin, 0, sizeof(enteredPin));
  pinIndex = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Code:");
}