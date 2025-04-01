#ifndef WEBSERVER_H
#define WEBSERVER_H
#include <ArduinoJson.h>
#include "storage.h"
#include "lcd.h"
#include "keyPadControll.h"
#include "transistor.h"

class WebServer
{
private:
    void notFound();

public:
    WebServer();
    void sendData();
    void handleWeb();

    string webPin;
    Storage &Pin;
    Storage &readPin();
    Storage &writeWiFiData();
    Storage &readWiFiData(); // we realy need this? we need to call this function to get ssid and password and connect to thid wifi with it every time whrn it has been powered on
    MyLCD &turnBackLight();
    KeyPadControll &isCorrectPin;
    Transistor &on();
    Transistor &off();
}

#endif