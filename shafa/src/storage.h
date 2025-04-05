#ifndef STORAGE_H
#define STORAGE_H
#include <ArduinoJson.h>
#include <LittleFS.h>
// #include "webServer.h"

class Storage
{
private:
    const String pinPath = "/pin.txt";
    // String ssid;
    // String password;

public:
    Storage();
    void StorageSetup();
    String pin;
    String readPin();
    String readedPin;
    void writePin(String &confirmPin);
    // void deletePin();
    // void checkFolder(const char *path);
    // void writeWiFiData();
    // void readWiFiData();
};

#endif
