#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include <Arduino.h>
#include "security.h"

class Transistor
{
public:
    Transistor(uint8_t pin);
    void on();
    void off();
    bool getState();
    void unlock();
    void handleUnlock();
    uint16_t UNLOCK_TIME = 2000;
private:
    uint8_t pin;
    bool state;
    uint32_t currentTime = 0;
};

#endif