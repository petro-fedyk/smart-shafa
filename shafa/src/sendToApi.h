#ifndef SENT_TO_API
#define SENT_TO_API
#include <Arduino.h>
#include <HTTPClient.h>
#include "convertToJson.h"

uint8_t debug = 0;

String createJsonString(const String &methdot, bool isSuccess);

const char *serverName = "http://192.168.31.21:5000/api/logs";

void testBasicConnectivity() {
    Serial.println("=== Testing Basic Connectivity ===");
    
    IPAddress serverIP;
    if (WiFi.hostByName("google.com", serverIP)) {
        Serial.printf("✓ DNS works - google.com: %s\n", serverIP.toString().c_str());
    } else {
        Serial.println("✗ DNS resolution failed");
    }
    
    if (WiFi.gatewayIP() != IPAddress(0, 0, 0, 0)) {
        Serial.printf("✓ Gateway reachable: %s\n", WiFi.gatewayIP().toString().c_str());
    }
    
    HTTPClient http;
    WiFiClient client;
    
    http.begin(client, "http://192.168.31.21:5000/");
    http.setTimeout(5000);
    
    int httpResponseCode = http.GET();
    Serial.printf("Basic server test (GET /): %d\n", httpResponseCode);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.printf("Server response: %s\n", response.c_str());
    }
    
    http.end();
    Serial.println("===================");
}  

void sendToSrver(const String &methdot, bool isSuccess)
{
    if (debug == 1) {
        Serial.println("=== WiFi Debug Info ===");
        Serial.print("WiFi Mode: ");
        Serial.println(WiFi.getMode() == WIFI_MODE_AP ? "AP" : "STA");
        Serial.print("WiFi Status: ");
        Serial.println(WiFi.status());
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
        Serial.print("Gateway: ");
        Serial.println(WiFi.gatewayIP());
        Serial.print("DNS: ");
        Serial.println(WiFi.dnsIP());
        Serial.println("=====================");
        testBasicConnectivity();
    }
    


    if (WiFi.getMode() != WIFI_MODE_STA) {
        Serial.println("ESP32 in AP mode - cannot reach external server");
        return;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected - cannot send data to server");
        return;
    }

    HTTPClient http;
    WiFiClient client;

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/json");
    extern Storage storage;
    String userPin = storage.readPin();
    http.addHeader("X-Auth-PIN", userPin);
    http.setTimeout(15000);

    String jsonStr = createJsonString(methdot, isSuccess);

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

        switch(httpResponseCode) {
            case -1: Serial.println("Connection failed"); break;
            case -2: Serial.println("Send header failed"); break;
            case -3: Serial.println("Send payload failed"); break;
            case -4: Serial.println("Not connected"); break;
            case -5: Serial.println("Connection lost"); break;
            case -6: Serial.println("No stream"); break;
            case -7: Serial.println("No HTTP server"); break;
            case -8: Serial.println("Too less RAM"); break;
            case -9: Serial.println("Encoding error"); break;
            case -10: Serial.println("Stream write error"); break;
            case -11: Serial.println("Read timeout"); break;
            default: Serial.println("Unknown error"); break;
        }
    }

    http.end();
}

#endif