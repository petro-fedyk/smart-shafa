#ifndef CONVERT_TO_JSON
#define CONVERT_TO_JSON

#include <Arduino.h>
#include <ArduinoJson.h>
#include "KeyPadControl.h"
#include "clock.h"

extern KeyPadControl keyPadControl;
extern MyClock myClock;

bool wasTried = false;

// Оголошення функцій
String checkMethod(KeyPadControl &control);
void printJson(const String &json);
String createJsonString(const String &buffer, const String &methdot, bool isSuccess);

void checkTryUnlock()
{
    if (keyPadControl.tryToUnlock && !wasTried)
    {
        wasTried = false;
        keyPadControl.tryToUnlock = false;

        String buffer = myClock.getFormattedDateTime();
        String methdot = checkMethod(keyPadControl);
        bool isSuccess = keyPadControl.isKeyUnlock; // Приклад: отримуємо статус успіху

        Serial.print("isKeyUnlock: ");
        Serial.println(keyPadControl.isKeyUnlock);
        Serial.print("isSuccess: ");
        Serial.println(isSuccess);

        String json = createJsonString(buffer, methdot, isSuccess);

        printJson(json);
    }
}

String checkMethod(KeyPadControl &control)
{
    String methdot = "";
    if (control.isKeyUnlock)
    {
        methdot = "KeyPad";
    }
    return methdot;
}

String createJsonString(const String &buffer, const String &methdot, bool isSuccess)
{
    StaticJsonDocument<200> doc; // Використовуйте StaticJsonDocument для JSON
    doc["time"] = buffer;
    doc["method"] = methdot;
    doc["isSuccess"] = isSuccess;

    String output;
    serializeJson(doc, output);
    return output;
}

void printJson(const String &json)
{
    Serial.println(json);
}

#endif