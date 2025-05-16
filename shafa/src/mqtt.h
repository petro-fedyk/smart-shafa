#ifndef MQTT_T
#define MQTT_T
#include <Arduino.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include "convertToJson.h"
#include "transistor.h"

const char *mqtt_broker = "192.168.1.119";
const char *topic = "shafaTopic";
const char *mqtt_username = "username";
const char *mqtt_password = "password";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
extern Transistor transistor;

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    if ((char)payload[0] == '1')
    {
        transistor.unlock();
    }
}
void sentData()
{
    if (!client.connected())
    {
        Serial.println("MQTT not connected, skipping publish");
        return;
    }

    String buffer = myClock.getFormattedDateTime();
    String method = checkMethod(keyPadControl);
    bool isSuccess = keyPadControl.isKeyUnlock;

    String json = createJsonString(buffer, method, isSuccess);

    Serial.print("Sending to MQTT: ");
    Serial.println(json);

    client.publish(topic, json.c_str());
}

void mqttSetUp()
{
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected())
    {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Connected to MQTT broker");
        }
        else
        {
            Serial.print("Failed to connect, state: ");
            Serial.println(client.state());
            delay(2000);
        }
    }
    client.subscribe(topic);
}

#endif