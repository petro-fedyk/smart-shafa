#ifndef STORAGE_H
#define STORAGE_H
#include <ArduinoJson.h>
#include <LittleFS.h>
// #include "webServer.h"

class Storage
{
private:
    const String pinPath = "/pin.txt";
    const String modePath = "/mode.txt";
    const String credentialsPath = "/wifi.csv";



    // String ssid;
    // String password;

public:
    void StorageSetup();
    String pin;
    String readPin();
    String readedPin;
    String readedssid;
    String readedpassword;
    void writePin(String &confirmPin);
    void readCredentials();
    void writeCredentials(String &ssid, String &password);
    String readedMode;
    String readMode();
    void writeMode(const String &mode);
    // void deletePin();
    // void checkFolder(const char *path);
    // void writeWiFiData();
    // void readWiFiData();
};

#endif
