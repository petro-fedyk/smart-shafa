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
    Storage &readWiFiData(); // we realy need this?
    MyLCD &turnBackLight();
    keyPadControll &isCorrectPin;
    Transistor &on();
    Transistor &off();
}

#endif