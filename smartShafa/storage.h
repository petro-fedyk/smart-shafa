#ifndef STORAGE_H
#define STORAGE_H
#include <ArduinoJson.h>
#include <LittleFS.h>

class Storage
{
private:
    const string pinPath;
    const string wifiPath;

public:
    Storage();
    char[4] pin;
    void readPin();
    void writePin();
    void deletePin();
    void checkFolder();
    void writeWiFiData();
    void readWiFiData();
};

#endif
