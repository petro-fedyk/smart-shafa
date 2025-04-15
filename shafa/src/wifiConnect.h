#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <WiFi.h>

void connectToWiFi(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 30000;

    Serial.println("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime < timeout))
    {
        if ((millis() - startAttemptTime) % 1000 < 100)
        {
            Serial.println("Still trying...");
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Failed to connect to WiFi");
    }
}

#endif
