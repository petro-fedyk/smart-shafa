#include "transistor.h"

Transistor::Transistor(uint8_t pin) : pin(pin), state(false), currentTime(0)
{
    pinMode(pin, OUTPUT);
    off();
}

void Transistor::on()
{
    if (!state)
    {
        digitalWrite(pin, HIGH);
        state = true;
    }
}

void Transistor::off()
{
    if (state)
    {
        digitalWrite(pin, LOW);
        state = false;
    }
}

bool Transistor::getState()
{
    return state;
}

void Transistor::unlock()
{
    if (!state)
    {
        on();
        currentTime = millis();
    }
}

void Transistor::handleUnlock(){
    if (state && (millis() - currentTime >= UNLOCK_TIME))
    {
        off();
    }
}