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
void Storage::writePin(String &newPin)
{
    File file = LittleFS.open(wifiPath, "r");
    String fileContent;
    if (file)
    {
        while (file.available())
        {
            fileContent += (char)file.read();
        }
        file.close();
    }
    else
    {
        Serial.println("Failed to open users file, creating a new one...");
        fileContent = "{\"users\":[]}";
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, fileContent);

    if (error)
    {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.f_str());
        return;
    }

    JsonArray users = doc["users"].as<JsonArray>();

    int newId = 1;
    if (users.size() > 0)
    {
        newId = users[users.size() - 1]["id"].as<int>() + 1;
    }

    JsonObject newUser = users.createNestedObject();
    newUser["id"] = newId;
    newUser["pin"] = newPin;

    String updatedJson;
    serializeJson(doc, updatedJson);

    file = LittleFS.open(filePath, "w");
    if (file)
    {
        file.print(updatedJson);
        file.close();
        Serial.println("New PIN added successfully!");
    }
    else
    {
        Serial.println("Failed to save updated users file!");
    }
}
void Storage::deletePin()
{
    // how and for what?
}
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

void Storage::writeWiFiData(String &ssid, String &password)
{
    // no logic to get ssid and password yet
    File file = LittleFS.open(wifiPath, "w");
    if (file)
    {
        file.printf("{\"ssid\": \"%s\", \"password\": \"%s\"}", ssid, password);
        file.close();
    }
    else
    {
        checkFolder();
    }
}
void Storage::readWiFiData()
{

    File file = LittleFS.open(wifiPath, "r");
    if (!file)
    {
        Serial.println("Failed to open WiFi config file");
        return;
    }

    String fileContent;
    while (file.available())
    {
        fileContent += (char)file.read();
    }
    file.close();

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, fileContent);

    if (error)
    {
        Serial.print("JSON deserialization failed: ");
        Serial.println(error.f_str());
        return;
    }

    String ssid = doc["ssid"].as<String>();
    String password = doc["password"].as<String>();

    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);

    // TODO: Store ssid and password in global variables or use them as needed
}
