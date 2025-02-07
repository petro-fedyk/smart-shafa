#ifndef STORAGE_H
#define STORAGE_H

#include "pin.h"
#include <stdint.h>
#include <EEPROM.h>
#include "Config.h"
#include "pin.h"

extern char hexaKeys[KEYPAD_ROWS][KEYPAD_COLS];

const int MAX_PASSWORDS = 5;
const int PASSWORD_LENGTH = 4;

class Storage
{
public:
    void begin();
    void savePassword(uint8_t index, const char *password, uint8_t length);
    void loadPassword(uint8_t index, char *buffer, uint8_t length);
    uint8_t getPasswordCount();
    void incrementPasswordCount();

private:
    uint8_t passwordCount = 0;
};

#endif
