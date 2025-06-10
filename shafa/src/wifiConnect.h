#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <WiFi.h>

void connectToWiFi(char *ssid, char *password, wifi_mode_t *mode)
{
    WiFi.disconnect();
    WiFi.mode(*mode);

    if (*mode == WIFI_MODE_STA)
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
            Serial.println("Switching to AP mode...");
            wifi_mode_t mode = WIFI_MODE_AP;
            connectToWiFi(ssid, password, &mode);
        }
    }
    else if (*mode == WIFI_MODE_AP)
    {
        WiFi.softAP(ssid, password);
        Serial.println("AP mode started");
        Serial.print("AP IP Address: ");
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println("Unsupported WiFi mode");
    }
}

#endif
