#include <Arduino.h>
#include <Keypad.h>
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

const char *ntpServer1 = "pool.ntp.org"; // Переміщено перед створенням myClock
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

MyClock myClock(&lcd, gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2); // Передаємо значення// Тепер змінні доступні
KeyPadControl keyPadControl(lcd, storage, transistor, myClock);

const char *WIFI_SSID = "admin";
const char *WIFI_PASSWORD = "domestos1216";

void setup()
{
  Serial.begin(115200);

  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);
  setupOTA("my_esp32", OTA_PIN);
  myClock.initClock();

  storage.StorageSetup();

  keyPadControl.keyPadSetup();
  Serial.println("Setup complete");
}

void loop()
{
  keyPadControl.keyPadLoop();
  if (transistor.isTransistorOpen())
  {
    transistor.unlock();
  }
  if (myClock.isClockShow)
  {
    myClock.updateClock();
  }
  handleOTA();
}