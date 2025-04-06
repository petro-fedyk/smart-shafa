#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
Storage storage;
Transistor transistor(TRANSISTOR_PIN); // Створюємо об'єкт Transistor
KeyPadControl keyPadControl(lcd, storage, transistor);

void setup()
{
  Serial.begin(115200);

  storage.StorageSetup();

  keyPadControl.keyPadSetup();
  Serial.println("Setup complete");
}

void loop()
{
  keyPadControl.keyPadLoop();
  // unlock();
  if (transistor.isTransistorOpen())
  {
    transistor.unlock(); // Викликаємо функцію unlock() для керування транзистором
  }
}

// дати функцію в транзисторі викликати коли треба відкрити двері