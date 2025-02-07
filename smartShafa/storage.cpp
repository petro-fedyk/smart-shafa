#include "Storage.h"
#include <EEPROM.h>

char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {BTN_CONFIRM, '0', BTN_RESET, 'D'}};

const uint8_t initialPassword[PASSWORD_LENGTH] = {'1', '2', '3', '4'};

void Storage::begin()
{
    EEPROM.begin(512);
    passwordCount = EEPROM.read(0);

    if (passwordCount == 0 || passwordCount > MAX_PASSWORDS)
    {
        savePassword(0, initialPassword, PASSWORD_LENGTH);
        incrementPasswordCount();
    }
}

void Storage::savePassword(uint8_t index, const uint8_t *password, int length)
{
    int address = 1 + index * PASSWORD_LENGTH;

    for (int i = 0; i < length; i++)
    {
        EEPROM.write(address + i, password[i]);
    }
    EEPROM.commit();
}

void Storage::loadPassword(uint8_t index, uint8_t *buffer, int length)
{
    int address = 1 + index * PASSWORD_LENGTH;

    for (int i = 0; i < length; i++)
    {
        buffer[i] = EEPROM.read(address + i);
    }
}

uint8_t Storage::getPasswordCount()
{
    return passwordCount;
}

void Storage::incrementPasswordCount()
{
    if (passwordCount < MAX_PASSWORDS)
    {
        passwordCount++;
        EEPROM.write(0, passwordCount);
        EEPROM.commit();
    }
}
