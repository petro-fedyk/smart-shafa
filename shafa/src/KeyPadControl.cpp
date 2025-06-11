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

KeyPadControl::KeyPadControl(LiquidCrystal_I2C &lcd, Storage &storage, Transistor &transistor, MyClock &RTclock, myBuzzer &buzzer)
    : customKeypad(makeKeymap(hexaKeys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS),
      lcd(lcd),
      storage(storage),
      transistor(transistor),
      RTclock(RTclock),
      buzzer(buzzer),
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
  lcdStateMachine(lcdState);
  Serial.println("KeyPad Setup complete");
}

void KeyPadControl::keyPadLoop()
{
  char key = customKeypad.getKey();
  isKeyPressed = (key != NO_KEY);

  if (key == BTN_REBOOT)
  {
    esp_restart();
  }
  if (key == BTN_BACKLIGHT)
  {
    clearPin();
    static bool backlightOn = true;
    backlightOn = !backlightOn;
    if (backlightOn)
    {
      lcd.backlight();
    }
    else
    {
      lcd.noBacklight();
    }
  }

  if (key)
  {
    lastKeyPressTime = millis();
    lcdStateMachine(lcdState);
    if (RTclock.isClockShow)
    {
      RTclock.isClockShow = false;
      RTclock.firstUpdate = true;
      clearPin();
      lcdStateMachine(lcdState);
    }
  }

  if (millis() - lastKeyPressTime > WAIT_TIME)
  {
    if (!RTclock.isClockShow)
    {
      Serial.println("No activity detected. Switching to clock mode...");
      lcdState = 8;
      lcdStateMachine(lcdState);
    }
    return;
  }

  if (key == CHANGE_PIN)
  {
    changePasswordMode = true;
    lcdState = 1;
    lcdStateMachine(lcdState);
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
        lcdState = 3;
        lcdStateMachine(lcdState);
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
        lcdState = 2;
        lcdStateMachine(lcdState);
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

          lcdState = 4;
          lcdStateMachine(lcdState);
          clearPin();
          tempPin = "";
          confirmPin = "";
          changePasswordStage = 0;
        }
        else
        {
          lcdState = 5;
          lcdStateMachine(lcdState);
          Serial.println("Password confirmation mismatch");
          clearPin();
        }
        clearPin();
        lcdStateMachine(lcdState);
        changePasswordMode = false;
      }
      else
      {
        lcdState = 6;
        lcdStateMachine(lcdState);
        clearPin();
        lcdStateMachine(lcdState);
        changePasswordMode = false;
      }
    }
    else if (key == BTN_RESET)
    {
      changePasswordMode = false;
      changePasswordStage = 0;
      clearPin();
      lcdStateMachine(lcdState);
    }
    else if (key != NO_KEY)
    {
      // String pinDisplay = enterPin();
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
    tryToUnlock = true;
    Serial.println("try to unlock");
    if (isUnlockCodeCorrect())
    {
      lcdState = 7;
      lcdStateMachine(lcdState);
      transistor.unlock();
      Serial.println("Access Granted");
      clearPin();
      lcdStateMachine(lcdState);

      buzzer.successSound();
      isKeyUnlock = true;
      isSuccess = true;
    }
    else
    {
      lcdState = 6;
      lcdStateMachine(lcdState);
      Serial.println("Wrong Code");
      clearPin();
      lcdStateMachine(lcdState);

      buzzer.unsuccessSound();
      isKeyUnlock = false;
      isSuccess = false;
    }
    clearPin();
    lcdStateMachine(lcdState);
  }
  else if (key == BTN_RESET)
  {
    clearPin();
    lcdStateMachine(lcdState);
  }
  else if (key != NO_KEY)
  {
    // String pinDisplay = enterPin();

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
  // tryToUnlock = false;
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
  lcd.setCursor(0, 0);
  lcdState = 0;
  // isKeyUnlock = false;
  // isSuccess = false;
  // tryToUnlock = false;
}

void KeyPadControl::lcdStateMachine(uint8_t &state)
{
  switch (state)
  {
  case 0:
    lcd.clear();
    lcd.print("Enter Code:");
    Serial.println("Enter Code:");
    break;
  case 1:
    lcd.clear();
    lcd.print("Enter Old Pass:");
    Serial.println("Enter Old Pass:");
    break;
  case 2:
    lcd.clear();
    lcd.print("Confirm New Pass:");
    Serial.println("Confirm New Pass:");
    break;
  case 3:
    lcd.clear();
    lcd.print("Enter New Pass:");
    Serial.println("Enter New Pass:");
    break;
  case 4:
    lcd.clear();
    lcd.print("Pass Changed");
    Serial.println("Pass Changed");
    break;
  case 5:
    lcd.clear();
    lcd.print("Mismatch");
    Serial.println("Mismatch");
    break;
  case 6:
    lcd.clear();
    lcd.print("Wrong Pass");
    Serial.println("Wrong Pass");
  case 7:
    lcd.clear();
    lcd.print("Access Granted");
    Serial.println("Access Granted");
    break;
  case 8:
    lcd.clear();
    RTclock.showClock();
    Serial.println("Clock is mode activated");
    break;
  default:
    Serial.println("Unhandled state");
    break;
  }
}