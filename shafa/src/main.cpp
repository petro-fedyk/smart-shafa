#include <Arduino.h>
#include <Keypad.h>
#include "security.h"
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"
#include "wifiConnect.h"
#include "clock.h"
#include "ota.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
Storage storage;
Transistor transistor(TRANSISTOR_PIN);

const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

MyClock myClock(&lcd, gmtOffset_sec, daylightOffset_sec, NTP_SERVER1, NTP_SERVER2); // Передаємо значення// Тепер змінні доступні
KeyPadControl keyPadControl(lcd, storage, transistor, myClock);

void setup()
{
  Serial.begin(115200);

  connectToWiFi(WIFI_SSID, PASSOWORD);
  setupOTA("my_esp32", OTA_PIN);
  myClock.initClock();

  storage.StorageSetup();

  keyPadControl.keyPadSetup();
  Serial.println("Setup complete");
}

void loop()
{
  keyPadControl.keyPadLoop();
  if (myClock.isClockShow)
  {
    myClock.updateClock();
  }
  handleOTA();
}