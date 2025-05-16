#ifndef SENT_TO_API
#define SENT_TO_API
#include <Arduino.h>
#include <HTTPClient.h>
#include "convertToJson.h"

const char *serverName = "http://192.168.0.129:5000//api/shafa_data/";

void sendToSrver(const String &buffer, const String &methdot, bool isSuccess)
{
    HTTPClient http;
    WiFiClient client;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonStr = createJsonString(buffer, methdot, isSuccess);

    int httpResponseCode = http.POST(jsonStr);

    Serial.print("Sending data: ");
    Serial.println(jsonStr);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.print("Response from server: ");
        Serial.println(response);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

#endif