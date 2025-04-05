#include "storage.h"

Storage::Storage()
{
    // Ініціалізація, якщо потрібна
    Serial.println("Storage initialized");
}

void Storage::StorageSetup()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    readPin();
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
            file.println("1234"); // write the default pin
            file.close();
            Serial.println("File created");
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

    // Видаляємо зайві символи (наприклад, символи нового рядка)
    readedPin.trim();

    Serial.println("File Content:");
    Serial.println(readedPin);

    return readedPin; // Повертаємо вміст файлу
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