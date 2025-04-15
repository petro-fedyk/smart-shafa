#include <Arduino.h>
#include <Keypad.h>
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"
#include "wifiConnect.h"
#include "clock.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
Storage storage;
Transistor transistor(TRANSISTOR_PIN);
MyClock myClock(&lcd, 72000, 3600, "pool.ntp.org", "time.nist.gov"); // Змінили ім'я об'єкта
KeyPadControl keyPadControl(lcd, storage, transistor, myClock);

const char *WIFI_SSID = "admin";
const char *WIFI_PASSWORD = "domestos1216";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 72000;
const int daylightOffset_sec = 3600;

void setup()
{
  Serial.begin(115200);

  connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

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
}