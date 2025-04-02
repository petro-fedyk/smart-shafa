#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"

void setup()
{
  Serial.begin(115200);
  KeyPadControl.keyPadSetup();
}

void loop()
{
  KeyPadControl.keyPadLoop();
}