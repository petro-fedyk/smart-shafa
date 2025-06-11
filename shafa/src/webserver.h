#ifndef WEBSERVR_H
#define WEBSERVR_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "storage.h"
#include "wifiConnect.h"
#include "KeyPadControl.h"
#include "transistor.h"

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
Storage *webStorage = nullptr;
Transistor *webTransistor = nullptr;
extern KeyPadControl keyPadControl;

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n",
                      client->id(), client->remoteIP().toString().c_str());

        {
            DynamicJsonDocument doc(1024);
            doc["type"] = "currentData";

            webStorage->readCredentials();
            webStorage->readMode();

            doc["data"]["ssid"] = webStorage->readedssid;
            doc["data"]["password"] = webStorage->readedpassword;
            doc["data"]["mode"] = webStorage->readedMode.toInt(); // 0=AP, 1=STA

            String message;
            serializeJson(doc, message);
            client->text(message);
        }
        break;

    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;

    case WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            data[len] = 0;
            String message = (char *)data;

            Serial.printf("Received WebSocket message: %s\n", message.c_str());

            DynamicJsonDocument doc(1024);
            DeserializationError error = deserializeJson(doc, message);

            if (error)
            {
                Serial.print("JSON parsing failed: ");
                Serial.println(error.c_str());
                return;
            }

            String command = doc["command"];
            DynamicJsonDocument response(512);
            response["command"] = command;
            response["timestamp"] = millis();

            if (command == "saveWiFiSettings")
            {
                String ssid = doc["ssid"];
                String password = doc["password"];
                String mode = doc["mode"]; // "0" for AP, "1" for STA

                if (ssid.length() == 0)
                {
                    response["status"] = "error";
                    response["message"] = "SSID cannot be empty";
                }
                else
                {
                    String ssidCopy = ssid;
                    String passwordCopy = password;
                    webStorage->writeCredentials(ssidCopy, passwordCopy);
                    webStorage->writeMode(mode);

                    response["status"] = "success";
                    response["message"] = "WiFi settings saved successfully. ESP will restart in 3 seconds...";
                    response["restart"] = true;

                    String responseStr;
                    serializeJson(response, responseStr);
                    ws.textAll(responseStr);

                    Serial.println("WiFi settings saved:");
                    Serial.println("SSID: " + ssid);
                    Serial.println("Password: " + password);
                    Serial.println("Mode: " + mode + " (0=AP, 1=STA)");
                    Serial.println("Restarting ESP32 in 3 seconds...");

                    delay(3000);
                    ESP.restart();
                    return;
                }
            }
            else if (command == "unlockDevice")
            {
                String enteredPIN = doc["pin"];

                String storedPIN = webStorage->readPin();
                if (enteredPIN == storedPIN)
                {
                    webTransistor->unlock();

                    sendToSrver("WEB", true);

                    response["status"] = "success";
                    response["message"] = "Device unlocked successfully";
                    response["unlocked"] = true;

                    Serial.println("Device unlocked via web interface with PIN: " + enteredPIN);
                }
                else
                {

                    response["status"] = "error";
                    response["message"] = "Invalid PIN - unlock failed";
                    response["unlocked"] = false;

                    Serial.println("Unlock failed - invalid PIN via web interface");
                }
            }
            else if (command == "getCurrentData")
            {
                DynamicJsonDocument dataDoc(1024);
                dataDoc["type"] = "currentData";

                webStorage->readCredentials();
                webStorage->readMode();

                dataDoc["data"]["ssid"] = webStorage->readedssid;
                dataDoc["data"]["password"] = webStorage->readedpassword;
                dataDoc["data"]["mode"] = webStorage->readedMode.toInt();

                String dataMessage;
                serializeJson(dataDoc, dataMessage);
                client->text(dataMessage);
                return;
            }
            else
            {
                response["status"] = "error";
                response["message"] = "Unknown command: " + command;
            }

            String responseStr;
            serializeJson(response, responseStr);
            ws.textAll(responseStr);
        }
    }
    break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    }
}

void initWebServer(Storage *storage, Transistor *transistor)
{
    webStorage = storage;
    webTransistor = transistor;

    if (!LittleFS.begin())
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    ws.onEvent(onWebSocketEvent);
    server.addHandler(&ws);

    server.on("/api/data", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        DynamicJsonDocument doc(512);
        
        webStorage->readCredentials();
        webStorage->readMode();
        
        doc["ssid"] = webStorage->readedssid;
        doc["password"] = webStorage->readedpassword;
        doc["mode"] = webStorage->readedMode.toInt();
        doc["hasPin"] = !webStorage->readPin().isEmpty();
        
        String response;
        serializeJson(doc, response);
        
        request->send(200, "application/json", response); });

    server.begin();
    Serial.println("WebServer started on port 80");
}

void handleWebServerClients()
{
    ws.cleanupClients();
}

#endif