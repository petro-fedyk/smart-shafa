#include "storage.h"

Storage::Storage();
void Storage::readPin()
{
    File file = LittleFS.open("/littleFS/pin.json", "r");
    if (!file) {
        Serial.println("Не вдалося відкрити users.json!");
        return;
    }

    DynamicJsonDocument doc(512);
    deserializeJson(doc, file);
    file.close();

    for (JsonVariant user : doc["users"].as<JsonArray>()) {
        Serial.printf("ID: %d, PIN: %s\n", user["id"].as<int>(), user["pin"].as<const char*>());
    }
}
void Storage::writePin();
void Storage::deletePin();
void Storage::checkFolder();
void Storage::writeWiFiData();
void Storage::readWiFiData();
