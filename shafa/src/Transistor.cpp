#include "transistor.h"

#define UNLOCK_TIME 2000

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
    u_int64_t now = millis();

    if (!state)
    {
        on();
        currentTime = now;
    }
    else if (now - currentTime >= UNLOCK_TIME)
    {
        off();
    }
}
