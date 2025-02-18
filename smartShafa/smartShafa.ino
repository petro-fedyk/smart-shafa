// #include <LiquidCrystal_I2C.h>
// #include <Keypad.h>
// #include "wifiConnect.h"
#include "keyPadControll.h"
// #include "clock.h"
#include "config.h"
#include "ota.h"
#include "pin.h"
// #include "security.h"
#include "transistor.h"
#include "lcd.h"
// #include "storage.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char *WIFI_SSID = SSID;
const char *WIFI_PASSWORD = PASSOWORD;

const char *ntpServer1 = NTP_SERVER1;
const char *ntpServer2 = NTP_SERVER2;
const long gmtOffset_sec = GTMOFFSET_SEC;
const int daylightOffset_sec = DAYLIGHTOFFSET;

String apiURL = API_URL;
String apiKey = API_KEY;
String latitude = LAT;
String longitude = LON;

void setup()
{
    Serial.begin(115200);
    connectToWiFi(WIFI_SSID, WIFI_PASSWORD);

    setupOTA("my_esp32", OTA_PIN);
}

void loop()
{

    handleOTA();
}