#include "storage.h"

void Storage::StorageSetup()
{
    Serial.println("Initializing LittleFS...");
    
    // Try multiple mount attempts with different configurations
    bool mounted = false;
    
    // First try: Normal mount
    if (LittleFS.begin()) {
        mounted = true;
        Serial.println("LittleFS mounted successfully (normal)");
    }
    // Second try: Mount with format on fail
    else if (LittleFS.begin(true)) {
        mounted = true;
        Serial.println("LittleFS mounted successfully (with format)");
    }
    // Third try: Explicit format then mount
    else {
        Serial.println("Formatting LittleFS...");
        if (LittleFS.format()) {
            if (LittleFS.begin()) {
                mounted = true;
                Serial.println("LittleFS mounted successfully (after format)");
            }
        }
    }
    
    if (!mounted) {
        Serial.println("LittleFS Mount Failed completely! Using defaults.");
        // Set default values in memory
        readedPin = "1234";
        readedssid = "ESPSETUP";
        readedpassword = "ESPSETUP";
        readedMode = "0";
        return;
    }
    
    // LittleFS is working, check and create files
    Serial.println("LittleFS mounted successfully");
    
    // Check total and used space
    Serial.printf("LittleFS Total: %d bytes, Used: %d bytes\n", 
                  LittleFS.totalBytes(), LittleFS.usedBytes());
    
    // Check if files exist, create them if they don't
    if (!LittleFS.exists("/pin.txt")) {
        String defaultPin = "1234";
        writePin(defaultPin);
        Serial.println("Created default PIN file");
    }
    
    if (!LittleFS.exists("/wifi.csv")) {
        String defaultSSID = "ESPSETUP";
        String defaultPassword = "ESPSETUP";
        writeCredentials(defaultSSID, defaultPassword);
        Serial.println("Created default WiFi credentials file");
    }
    
    if (!LittleFS.exists("/mode.txt")) {
        writeMode("0"); // Default to AP mode
        Serial.println("Created default mode file");
    }
    
    // List all files for debugging
    listFiles();
}

void Storage::listFiles() {
    Serial.println("Files in LittleFS:");
    File root = LittleFS.open("/");
    if (root) {
        File file = root.openNextFile();
        while (file) {
            Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
            file = root.openNextFile();
        }
        root.close();
    } else {
        Serial.println("Failed to open root directory");
    }
}

String Storage::readPin()
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, using default PIN");
        return "1234";
    }
    
    if (!LittleFS.exists("/pin.txt")) {
        Serial.println("PIN file doesn't exist, creating default");
        String defaultPin = "1234";
        writePin(defaultPin);
        return defaultPin;
    }
    
    File file = LittleFS.open("/pin.txt", "r");
    if (!file) {
        Serial.println("Failed to open PIN file");
        return "1234";
    }
    
    String pin = file.readString();
    file.close();
    pin.trim();
    
    Serial.println("Read PIN: " + pin);
    readedPin = pin;
    return pin;
}

void Storage::writePin(String &newPin)
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, cannot write PIN");
        return;
    }
    
    File file = LittleFS.open("/pin.txt", "w");
    if (!file) {
        Serial.println("Failed to open PIN file for writing");
        return;
    }
    
    file.print(newPin);
    file.close();
    Serial.println("PIN written: " + newPin);
    readedPin = newPin;
}

void Storage::readCredentials()
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, using default credentials");
        readedssid = "ESPSETUP";
        readedpassword = "ESPSETUP";
        return;
    }
    
    if (!LittleFS.exists("/wifi.csv")) {
        Serial.println("WiFi file doesn't exist, creating default");
        String defaultSSID = "ESPSETUP";
        String defaultPassword = "ESPSETUP";
        writeCredentials(defaultSSID, defaultPassword);
        readedssid = defaultSSID;
        readedpassword = defaultPassword;
        return;
    }
    
    File file = LittleFS.open("/wifi.csv", "r");
    if (!file) {
        Serial.println("Failed to open WiFi file");
        readedssid = "ESPSETUP";
        readedpassword = "ESPSETUP";
        return;
    }
    
    String line = file.readStringUntil('\n');
    file.close();
    
    int commaIndex = line.indexOf(',');
    if (commaIndex > 0) {
        readedssid = line.substring(0, commaIndex);
        readedpassword = line.substring(commaIndex + 1);
        readedssid.trim();
        readedpassword.trim();
    } else {
        readedssid = "ESPSETUP";
        readedpassword = "ESPSETUP";
    }
    
    Serial.println("Read SSID: " + readedssid);
    Serial.println("Read Password: " + readedpassword);
}

void Storage::writeCredentials(String &ssid, String &password)
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, cannot write credentials");
        return;
    }
    
    File file = LittleFS.open("/wifi.csv", "w");
    if (!file) {
        Serial.println("Failed to open WiFi file for writing");
        return;
    }
    
    file.print(ssid + "," + password);
    file.close();
    Serial.println("WiFi credentials written: " + ssid + " / " + password);
    readedssid = ssid;
    readedpassword = password;
}

String Storage::readMode()
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, using default mode");
        readedMode = "0";
        return "0";
    }
    
    if (!LittleFS.exists("/mode.txt")) {
        Serial.println("Mode file doesn't exist, creating default");
        writeMode("0");
        return "0";
    }
    
    File file = LittleFS.open("/mode.txt", "r");
    if (!file) {
        Serial.println("Failed to open mode file");
        readedMode = "0";
        return "0";
    }
    
    String mode = file.readString();
    file.close();
    mode.trim();
    
    Serial.println("Read mode: " + mode);
    readedMode = mode;
    return mode;
}

void Storage::writeMode(const String &mode)
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS not available, cannot write mode");
        return;
    }
    
    File file = LittleFS.open("/mode.txt", "w");
    if (!file) {
        Serial.println("Failed to open mode file for writing");
        return;
    }
    
    file.print(mode);
    file.close();
    Serial.println("Mode written: " + mode);
    readedMode = mode;
}