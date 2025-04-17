#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include <Arduino.h>

class Transistor
{
public:
    Transistor(uint8_t pin);
    void on();
    void off();
    bool getState();
    void unlock();

    void setTransistorOpen(bool state);
private:
    uint8_t pin;
    bool state;
    unsigned long currentTime = 0;
    bool TransistorOpen = false; // Прапорець для контролю стану
};

#endif