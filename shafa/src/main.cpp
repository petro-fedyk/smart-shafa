#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
KeyPadControl keyPadControl(lcd);

void setup()
{
  Serial.begin(115200);
  keyPadControl.keyPadSetup();
  Serial.println("Setup complete");
  pinMode(TRANSISTOR_PIN, OUTPUT);
  digitalWrite(TRANSISTOR_PIN, LOW); // Вимкнути транзистор
  Serial.println("Transistor pin set to LOW");
}

void loop()
{
  keyPadControl.keyPadLoop();
}