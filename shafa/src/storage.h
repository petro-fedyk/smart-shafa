#ifndef STORAGE_H
#define STORAGE_H
#include <ArduinoJson.h>
#include <LittleFS.h>
// #include "webServer.h"

class Storage
{
private:
    const String pinPath = "/pin.json";
    const String wifiPath = "/wifi.json";
    const String dir = "littleFS";
    // String ssid;
    // String password;

public:
    Storage();
    String pin;
    void readPin();
    // void writePin(String &newPin);
    // void deletePin();
    // void checkFolder(const char *path);
    // void writeWiFiData();
    // void readWiFiData();
};

#endif
