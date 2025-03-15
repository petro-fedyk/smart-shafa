#include "webServer.h"

void WebServer::notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void WebServer::sendData()
{
}
void WebServer::handleWeb()
{
    server.on("/backlight_false", "HTTP_GET", [](AsyncWebServerRequest * request))
    {
        Serial.print("backlight off");
        MyLCD &backlightState = false;
    }

    server.on("/backlight_true", "HTTP_GET", [](AsyncWebServerRequest * request))
    {
        Serial.print("backlight on");
        MyLCD &backlightState = on;
    }

    server.on("/pin", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        if (request->hasParam("value"))
        {
            webPin = request->getParam("value")->value();
            Serial.printf("Entered Pin: %s\n", webPin.c_str());
    
            StaticJsonDocument<100> jsonDoc;
            jsonDoc["pin"] = webPin;
    
            String jsonResponse;
            serializeJson(jsonDoc, jsonResponse);
            request->send(200, "application/json", jsonResponse);
        }
        else
        {
            request->send(400, "application/json", "{\"error\":\"Missing value parameter\"}");
        } 
        if(KeyPadControll &isCorrectPin())
        {
            Transistor &on();
        } 
        else
        {
            Transistor &off();
        } })

        server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request)
                  {
        if (request->hasParam("ssid") && request->hasParam("password"))
        {
            String ssidValue = request->getParam("ssid")->value();
            String passwordValue = request->getParam("password")->value();

            ssid = ssidValue;
            password = passwordValue;

            Serial.printf("SSID: %s\n", ssid.c_str());
            Serial.printf("Password: %s\n", password.c_str());

            StaticJsonDocument<200> jsonDoc;
            jsonDoc["ssid"] = ssid;
            jsonDoc["password"] = password;

            String jsonResponse;
            serializeJson(jsonDoc, jsonResponse);
            request->send(200, "application/json", jsonResponse);
        }
        else
        {
            request->send(400, "application/json", "{\"error\":\"Missing ssid or password\"}");
        } 
        Storage *writeWiFiData(ssid, password); })
}
