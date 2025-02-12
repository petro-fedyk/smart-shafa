#include "storage.h"

Storage::Storage();
void Storage::readPin()
{
    checkFolder(&pinPath);

    File file = LittleFS.open("/littleFS/pin.json", "r");

    DynamicJsonDocument doc(512);
    deserializeJson(doc, file);
    file.close();

    for (JsonVariant user : doc["users"].as<JsonArray>())
    {
        Serial.printf("ID: %d, PIN: %s\n", user["id"].as<int>(), user["pin"].as<const char *>());
    }
    return pin;
}
void Storage::writePin();
void Storage::deletePin();
void Storage::checkFolder()
{
    if (!LittleFS.exists(dir))
    {
        Serial.print("Folder is not created");
        LittleFS.mkdir(dir);
        Serial.print("Folder create");
        checkFolder();
    }
    else
    {
        if (!LittleFS.exist(pinPath))
            File file = LittleFS.open(pinPath, "w");
        file.println("{\"users\":[{\"id\":1,\"pin\":\"1234\"},{\"id\":2,\"pin\":\"5678\"}]}");

        file.close();

        if (!LittleFS.exist(wifiPath))
            File file = LITTLEFS.open(wifiPath, "w");

        file.close();
    }
}

void Storage::writeWiFiData();
void Storage::readWiFiData();
