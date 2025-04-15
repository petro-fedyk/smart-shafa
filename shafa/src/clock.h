// clock.h
#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "time.h"

class MyClock
{
private:
    LiquidCrystal_I2C *lcd;
    const long gmtOffset_sec;
    const int daylightOffset_sec;
    const char *ntpServer1;
    const char *ntpServer2;

    void setFont();
    void drawDigit(int col, int digit);
    void displayTime(int hours, int minutes);

public:
    MyClock(LiquidCrystal_I2C *lcd, long gmtOffset_sec, int daylightOffset_sec,
            const char *ntpServer1, const char *ntpServer2);

    void showClock();
    void initClock();

    void updateClock();
    bool isClockShow = false;
};

#endif
