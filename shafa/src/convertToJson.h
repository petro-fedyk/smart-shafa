#ifndef CONVERT_TO_JSON
#define CONVERT_TO_JSON

#include <Arduino.h>
#include <ArduinoJson.h>
#include "KeyPadControl.h"
#include "clock.h"
// #include "sendToApi.h"

extern KeyPadControl keyPadControl;
extern MyClock myClock;

bool wasTried = false;

String checkMethod(KeyPadControl &control);
void printJson(const String &json);
String createJsonString(const String &buffer, const String &methdot, bool isSuccess);
void sendToSrver(const String &buffer, const String &methdot, bool isSuccess);
// void sentData();

void checkTryUnlock()
{
    if (keyPadControl.tryToUnlock && !wasTried)
    {
        wasTried = false;
        keyPadControl.tryToUnlock = false;
        keyPadControl.isSuccess = false;
        keyPadControl.isKeyUnlock = false;

        String buffer = myClock.getFormattedDateTime();
        String methdot = checkMethod(keyPadControl);
        bool isSuccess = keyPadControl.isKeyUnlock;

        Serial.print("isKeyUnlock: ");
        Serial.println(keyPadControl.isKeyUnlock);
        Serial.print("isSuccess: ");
        Serial.println(isSuccess);

        String json = createJsonString(buffer, methdot, isSuccess);
        sendToSrver(buffer, methdot, isSuccess);
        // sentData();

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