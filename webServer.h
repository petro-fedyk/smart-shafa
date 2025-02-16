#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <ArduinoJson.h>
#include "storage.h"

class WebServer
{
private:
public:
    void sendData();
    void handleWeb();
    Storage &readPin();
    Storage &writeWiFiData();
    Storage &readWiFiData();
}

#endif