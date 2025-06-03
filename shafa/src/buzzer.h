#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class myBuzzer
{
private:
    uint8_t buzzPin;

    uint8_t gratingTone = 3;
    unsigned long frequencyGR = 1000;
    unsigned long timeGR = 300;

    uint8_t successTone = 3;
    unsigned long frequencySUC = 1200;
    unsigned long timeSUC = 150;

    uint8_t unsuccessTone = 2;
    unsigned long frequencyUnSUC = 300;
    unsigned long timeUnSUC = 400;

    uint8_t alarmTone = 2;
    unsigned long frequencyAlarm = 300;
    unsigned long timeAlarm = 12000;

public:
    myBuzzer(uint8_t pin);
    void greatingSound();
    void successSound();
    void unsuccessSound();
    void alarmSound();
};

#endif
