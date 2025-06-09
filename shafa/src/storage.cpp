#include "storage.h"

void Storage::StorageSetup()
{
    if (!LittleFS.begin(true, "/littlefs", 10U, "littlefs"))
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }
    File root = LittleFS.open("/");
    if (!root || !root.isDirectory())
    {
        Serial.println("Failed to open directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        Serial.print("FILE: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }
    readPin();
    readCredentials();
    readMode();
}

String Storage::readPin()
{
    File file = LittleFS.open(pinPath, "r");
    if (!file)
    {
        Serial.println("File not found, creating new one...");
        file = LittleFS.open(pinPath, "w");
        if (file)
        {
            file.println("1234");
            file.close();
            Serial.println("File created");
            readedPin = "1234";
            return readedPin;
        }
        else
        {
            Serial.println("Failed to create file");
            return "";
        }
    }

    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return "";
    }

    readedPin = "";
    while (file.available())
    {
        readedPin += (char)file.read();
    }
    file.close();

    readedPin.trim();

    Serial.println("File Content:");
    Serial.println(readedPin);

    return readedPin;
}

void Storage::writePin(String &newPin)
{
    File file = LittleFS.open(pinPath, "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    file.println(newPin);
    file.close();
    Serial.println("New PIN written to file");
}

void Storage::readCredentials()
{
    File file = LittleFS.open(credentialsPath, "r");
    if (!file)
    {
        Serial.println("File not found, creating new one...");
        file = LittleFS.open(credentialsPath, "w");
        if (file)
        {
            file.println("ESPSETUP,ESPSETUP");
            file.close();
            readedssid = "ESPSETUP";
            readedpassword = "ESPSETUP";
            Serial.println("File created");
        }
        else
        {
            Serial.println("Failed to create file");
            return;
        }
    }

    readedssid = "";
    readedpassword = "";

    if (file.available())
    {
        String line = file.readStringUntil('\n');
        int commaIndex = line.indexOf(',');
        if (commaIndex > 0)
        {
            readedssid = line.substring(0, commaIndex);
            readedpassword = line.substring(commaIndex + 1);
        }
        else
        {
            Serial.println("Invalid format in credentials file");
            file.close();
            return;
        }
        
    }
    file.close();

    Serial.println("ssid:");
    Serial.println(readedssid);
    Serial.println("password:");
    Serial.println(readedpassword);

}

String Storage::readMode()
{
    File file = LittleFS.open(modePath, "r");
    if (!file)
    {
        Serial.println("File not found, creating new one...");
        file = LittleFS.open(modePath, "w");
        if (file)
        {
            file.println("0");
            file.close();
            Serial.println("File created");
            readedMode = "0"; // Default mode
            return readedMode;
        }
        else
        {
            Serial.println("Failed to create file");
            return "";
        }
    }

    readedMode = file.readStringUntil('\n');
    file.close();

    readedMode.trim();

    Serial.println("Mode:");
    Serial.println(readedMode);

    return readedMode;
}

void Storage::writeMode(const String &mode)
{
    File file = LittleFS.open(modePath, "w");
    if (!file)
    {
        Serial.println("Failed to open mode file for writing");
        return;
    }

    file.println(mode);
    file.close();
    Serial.println("Mode written to file");
}