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
}

void loop()
{
  keyPadControl.keyPadLoop();
}