#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"

#define UNLOCL_TIME 2000
const unsigned long currentTime = 0;

void unlock();

LiquidCrystal_I2C lcd(0x27, 16, 2);
Storage storage;
KeyPadControl keyPadControl(lcd, storage);
// Transistor Transistor(uint8_t pin);

void setup()
{
  Serial.begin(115200);

  storage.StorageSetup();

  keyPadControl.keyPadSetup();
  Serial.println("Setup complete");
  pinMode(TRANSISTOR_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_PIN, LOW);
  Serial.println("Transistor pin set to LOW");
}

void loop()
{
  keyPadControl.keyPadLoop();
}

// void unlock()
// {
//   if (keyPadControl.openTransistor)
//   {
//     if (currentTime - millis() >= UNLOCL_TIME)
//     {
//       millis();
//       Transistor.on();
//     }
//   }
// }