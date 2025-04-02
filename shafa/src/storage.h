#ifndef STORAGE_H
#define STORAGE_H
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "webServer.h"

class Storage
{
private:
    const string *pinPath = "/pin.json";
    const string *wifiPath = "/wifi.json";
    const string *dir = "littleFS";
    const string *ssid;
    const string *password;

public:
    Storage();
    char pin[4];
    void readPin();
    void writePin();
    void deletePin();
    void checkFolder(const char *path);
    void writeWiFiData();
    void readWiFiData();
};

#endif
