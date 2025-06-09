#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "security.h"
#include "KeyPadControl.h"
#include "config.h"
#include "pin.h"
#include "transistor.h"
#include "storage.h"
#include "wifiConnect.h"
#include "clock.h"
#include "ota.h"
#include "convertToJson.h"
#include "sendToApi.h"
#include "buzzer.h"
// #include "mqtt.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

Storage storage;
Transistor transistor(TRANSISTOR_PIN);
myBuzzer buzzer(BUZZER_PIN);

const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

MyClock myClock(&lcd, gmtOffset_sec, daylightOffset_sec, NTP_SERVER1, NTP_SERVER2);
KeyPadControl keyPadControl(lcd, storage, transistor, myClock, buzzer);

wifi_mode_t wifi_mode = WIFI_MODE_AP;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println(WIFI_SSID);
  Serial.println(PASSWORD);
  connectToWiFi(WIFI_SSID, PASSWORD, &wifi_mode);
  setupOTA("my_esp32", OTA_PIN);

  // screenInit();

  myClock.initClock();

  storage.StorageSetup();

  keyPadControl.keyPadSetup();
  // mqttSetUp();
  Serial.println("Setup complete");
  buzzer.greatingSound();
}

void loop()
{
  transistor.handleUnlock();
  keyPadControl.keyPadLoop();
  if (myClock.isClockShow)
  {
    myClock.updateClock();
  }
  handleOTA();
  checkTryUnlock();

  // if (!client.connected())
  // {
  //   mqttSetUp(); // повторне підключення, якщо втрачено
  // }
  // client.loop();

  // String dataTime = myClock.getFormattedDateTime();
  // Serial.println(dataTime);
}