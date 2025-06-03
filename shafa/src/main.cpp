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
#include "alarm.h"
// #include "mqtt.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

Storage storage;
Transistor transistor(TRANSISTOR_PIN);
myBuzzer buzzer(BUZZER_PIN);

const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

MyClock myClock(&lcd, gmtOffset_sec, daylightOffset_sec, NTP_SERVER1, NTP_SERVER2);
KeyPadControl keyPadControl(lcd, storage, transistor, myClock, buzzer);

#define ALARM_UPDATE 10000
unsigned long curAlarmTime = 0;
bool previousAlarm = false;

void turnAlarm()
{
  if (millis() - curAlarmTime >= ALARM_UPDATE)
  {

    if (isAlarm && !previousAlarm)
    {
      buzzer.alarmSound();
      transistor.unlock();
      Serial.println("Alarm state changed");
    }
    previousAlarm = isAlarm;
    curAlarmTime = millis();
  }
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println(WIFI_SSID);
  Serial.println(PASSWORD);
  connectToWiFi(WIFI_SSID, PASSWORD);
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
  checkAlarm();
  turnAlarm();

  // if (!client.connected())
  // {
  //   mqttSetUp(); // повторне підключення, якщо втрачено
  // }
  // client.loop();

  // String dataTime = myClock.getFormattedDateTime();
  // Serial.println(dataTime);
}