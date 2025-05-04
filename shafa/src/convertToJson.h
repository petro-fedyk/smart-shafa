// unlockTime, unlockMethod, userData, isSuccess
#ifndef CONVERT_TO_JSON
#define CONVERT_TO_JSON
#include <Arduino.h>
#include <ArduinoJson.h>
#include "KeyPadControl.h"
#include "clock.h"

String checkMethod(KeyPadControl &control)
{
    String methdot = "";
    if (control.isKeyUnlock)
    {
        methdot = "KeyPad";
    }
    return methdot;
}

String convertToJson(const String &buffer, const String &methdot, bool isSuccess)
{
    JsonDocument doc;
    doc["time"] = buffer;
    doc["methdot"] = methdot;
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
