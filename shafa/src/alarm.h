#ifndef ALARM_H
#define ALARM_H
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "buzzer.h"
#include "api_key.h"

unsigned long lastTime = 0;
unsigned long timerDelay = 60000;
bool isAlarm = false;

void checkAlarm()
{
    if ((millis() - lastTime) > timerDelay)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;

            http.begin(API_URL);
            http.addHeader("accept", "application/json");
            http.addHeader("Authorization", API_KEY);

            int httpResponseCode = http.GET();

            if (httpResponseCode == 200)
            {
                String payload = http.getString();

                if (payload.length() > 10) // Захист від порожніх відповідей або коротких
                {
                    Serial.print("HTTP Response code: ");
                    Serial.println(httpResponseCode);
                    Serial.println(payload);

                    // Розбір JSON
                    const size_t capacity = 1024;
                    DynamicJsonDocument doc(capacity);

                    DeserializationError error = deserializeJson(doc, payload);
                    if (!error)
                    {
                        JsonArray root = doc.as<JsonArray>();
                        if (!root.isNull() && root.size() > 0)
                        {
                            JsonArray activeAlerts = root[0]["activeAlerts"];
                            bool newAlarm = activeAlerts.size() > 0;

                            if (isAlarm != newAlarm)
                            {
                                isAlarm = newAlarm;
                                Serial.print("Alarm status changed: ");
                                Serial.println(isAlarm ? "TRUE" : "FALSE");
                            }
                        }
                        else
                        {
                            Serial.println("Invalid or empty root array — keeping old alarm status");
                        }
                    }
                    else
                    {
                        Serial.print("JSON parse failed: ");
                        Serial.println(error.c_str());
                        Serial.println("Keeping previous alarm status");
                    }
                }
                else
                {
                    Serial.println("Empty or invalid payload — keeping previous alarm status");
                }
            }
            else
            {
                Serial.print("Error HTTP code: ");
                Serial.println(httpResponseCode);
                Serial.println("Keeping previous alarm status");
            }

            http.end();
        }
        Serial.println(isAlarm ? "TRUE" : "FALSE");

        lastTime = millis();
    }
}
#endif
