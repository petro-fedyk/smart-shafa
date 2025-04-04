#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
Storage storage;
KeyPadControl keyPadControl(lcd, storage);

void setup()
{
  Serial.begin(115200);

  if (!LittleFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File file = LittleFS.open("/text.txt", "r");
  if (!file)
  {
    Serial.println("File not found, creating new one...");
    file = LittleFS.open("/text.txt", "w");
    if (file)
    {
      file.println("1234");
      file.close();
      Serial.println("File created");
    }
    else
    {
      Serial.println("Failed to create file");
    }
  }

  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("File Content:");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();

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