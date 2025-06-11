#ifndef CONVERT_TO_JSON
#define CONVERT_TO_JSON

#include <Arduino.h>
#include <ArduinoJson.h>
#include "KeyPadControl.h"
#include "sendToApi.h"
#include "security.h"

extern KeyPadControl keyPadControl;

bool wasTried = false;

String checkMethod(KeyPadControl &control);
void printJson(const String &json);
String createJsonString(const String &methdot, bool isSuccess);
void sendToSrver(const String &methdot, bool isSuccess);
void sentData();

void checkTryUnlock()
{
    if (keyPadControl.tryToUnlock && !wasTried)
    {
        wasTried = false;
        keyPadControl.tryToUnlock = false;
        keyPadControl.isSuccess;
        keyPadControl.isKeyUnlock;

        String methdot = checkMethod(keyPadControl);
        bool isSuccess = keyPadControl.isKeyUnlock;

        Serial.print("isKeyUnlock: ");
        Serial.println(keyPadControl.isKeyUnlock);
        Serial.print("isSuccess: ");
        Serial.println(isSuccess);

        String json = createJsonString(methdot, isSuccess);
        sendToSrver(methdot, isSuccess);
        
        // sentData();
        
        keyPadControl.isSuccess = false;
        keyPadControl.isKeyUnlock = false;

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

String createJsonString(const String &method, bool isSuccess)
{
    DynamicJsonDocument doc(256);
    
    doc["device_id"] = DEVICE_ID;
    Serial.print("Device ID: ");
    Serial.println(DEVICE_ID);
    Serial.print("Method: ");
    Serial.println(method);
    if (method == "KeyPad") {
        doc["method_id"] = 1;
    } else if (method == "WEB") {
        doc["method_id"] = 2;
    } else {
        doc["method_id"] = 1;
    }
    
    doc["is_success"] = isSuccess;

    String output;
    serializeJson(doc, output);
    return output;
}

void printJson(const String &json)
{
    Serial.println(json);
}

#endif