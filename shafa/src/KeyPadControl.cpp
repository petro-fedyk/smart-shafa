#include "KeyPadControl.h"
#include "config.h"
#define WAIT_TIME 5000

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'E', '0', 'F', 'D'}};

// const char initialPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4'};
// const String PIN = "1234";

uint8_t rowPins[KEYPAD_ROWS] = {PIN_ROW_1, PIN_ROW_2, PIN_ROW_3, PIN_ROW_4};
uint8_t colPins[KEYPAD_COLS] = {PIN_COL_1, PIN_COL_2, PIN_COL_3, PIN_COL_4};

KeyPadControl::KeyPadControl(LiquidCrystal_I2C &lcd, Storage &storage, Transistor &transistor, MyClock &clock)
    : customKeypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS),
      lcd(lcd),
      storage(storage),
      transistor(transistor),
      clock(clock),
      pinIndex(0),
      changePasswordMode(false),
      changePasswordStage(0)
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
  isKeyPressed = (key != NO_KEY);

  if (key == BTN_REBOOT)
  {
    // ESP.restart();
    esp_restart();
  }
  if (key == BTN_BACKLIGHT)
  {
    backlight = !backlight;

    if (backlight)
    {
      lcd.backlight();
    }
    else
    {
      lcd.noBacklight();
    }

    return;
  }

  if (key)
  {
    lastKeyPressTime = millis();

    if (clock.isClockShow)
    {
      clock.isClockShow = false;
      clearPin();
    }
  }

  if (millis() - lastKeyPressTime > WAIT_TIME)
  {
    if (!clock.isClockShow)
    {
      Serial.println("No activity detected. Switching to clock mode...");
      clock.showClock();
    }
    return;
  }

  if (key == CHANGE_PIN)
  {
    changePasswordMode = true;
    lcd.clear();
    lcd.print("Enter Old Pass:");
    Serial.println("Change password mode activated");
    clearPin();
    return;
  }

  if (changePasswordMode)
  {
    if (key == BTN_CONFIRM)
    {
      if (pinIndex == PASSWORD_LENGTH && changePasswordStage == 0 && isUnlockCodeCorrect())
      {
        lcd.clear();
        lcd.print("Enter New Pass:");
        clearPin();
        changePasswordStage = 1;
      }
      else if (changePasswordStage == 1 && pinIndex == PASSWORD_LENGTH)
      {
        tempPin = "";
        for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
        {
          tempPin += char(enteredPin[i]);
        }
        lcd.clear();
        lcd.print("Confirm New Pass:");
        clearPin();
        changePasswordStage = 2;
      }
      else if (changePasswordStage == 2 && pinIndex == PASSWORD_LENGTH)
      {
        String confirmPin = "";
        for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
        {
          confirmPin += char(enteredPin[i]);
        }

        if (tempPin == confirmPin)
        {
          // * currently works but plz FIXME
          Serial.println("Password changed successfully");
          storage.writePin(confirmPin);
          Serial.print("New PIN: ");
          Serial.println(confirmPin);

          lcd.clear();
          lcd.print("Pass Changed");
          clearPin();
          tempPin = "";
          confirmPin = "";
          changePasswordStage = 0;
        }
        else
        {
          lcd.clear();
          lcd.print("Mismatch");
          Serial.println("Password confirmation mismatch");
          clearPin();
        }
        clearPin();
        changePasswordMode = false;
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
    else if (key != NO_KEY)
    {
      if (pinIndex < PASSWORD_LENGTH)
      {
        enteredPin[pinIndex++] = key;

        String pinDisplay = "";
        for (uint8_t i = 0; i < pinIndex; i++)
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
      transistor.unlock();
      lcd.print("Access Granted");
      Serial.println("Access Granted");
      clearPin();
    }
    else
    {
      lcd.clear();
      lcd.print("Wrong Code");
      Serial.println("Wrong Code");
      clearPin();
    }
    clearPin();
  }
  else if (key == BTN_RESET)
  {
    clearPin();
  }
  else if (key != NO_KEY)
  {
    if (pinIndex < PASSWORD_LENGTH)
    {
      enteredPin[pinIndex++] = key;

      String pinDisplay = "";
      for (uint8_t i = 0; i < pinIndex; i++)
      {
        pinDisplay += char(enteredPin[i]);
      }

      lcd.setCursor(0, 1);
      lcd.print(pinDisplay);
    }
  }
}

bool KeyPadControl::isUnlockCodeCorrect()
{
  readedPin = storage.readPin();

  String enteredPassword = "";
  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    enteredPassword += char(enteredPin[i]);
  }

  Serial.print("Entered password: ");
  Serial.println(enteredPassword);

  Serial.print("Stored password: ");
  Serial.println(readedPin);

  if (enteredPassword == readedPin)
  {
    Serial.println("true pin");
    return true;
  }
  else
  {
    Serial.println("false pin");
    return false;
  }
}

void KeyPadControl::clearPin()
{
  String confirmPin = "";
  String pinDisplay = "";

  for (uint8_t i = 0; i < PASSWORD_LENGTH; i++)
  {
    enteredPin[i] = 0;
  }

  pinIndex = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Code:");
}