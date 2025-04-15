
#include "clock.h"

const byte square_two_00[8] PROGMEM = {B11111, B11111, B11000, B00000, B00000, B00000, B00000, B00000};
const byte square_two_01[8] PROGMEM = {B00000, B00000, B00000, B00000, B00000, B11000, B11111, B11111};
const byte square_two_02[8] PROGMEM = {B11111, B11111, B00011, B00011, B00011, B00011, B11111, B11111};
const byte square_two_03[8] PROGMEM = {B11111, B11111, B11000, B11000, B11000, B11000, B11111, B11111};
const byte square_two_04[8] PROGMEM = {B00001, B00011, B00111, B00000, B00000, B00000, B00000, B00000};
const byte square_two_05[8] PROGMEM = {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
const byte square_two_06[8] PROGMEM = {B11111, B11111, B00000, B00000, B00000, B00000, B11111, B11111};
const byte square_two_07[8] PROGMEM = {B11000, B11000, B11000, B11000, B11000, B11000, B11111, B11111};

byte square_two_digits[10][4] = {
    {254, 254, 3, 2}, {4, 5, 254, 5}, {6, 2, 3, 6}, {0, 2, 1, 2}, {7, 1, 254, 5}, {3, 6, 6, 2}, {3, 6, 3, 2}, {0, 2, 254, 5}, {3, 2, 3, 2}, {3, 2, 6, 2}};

byte current_font_digits[10][4];
byte buffer[8];

void MyClock::showClock()
{
    Serial.println("Entering showClock()"); // Лог для перевірки виклику функції

    if (!isClockShow)
    {
        lcd->clear();                           // Очищаємо дисплей
        isClockShow = true;                     // Установлюємо прапорець
        Serial.println("Clock mode activated"); // Лог для підтвердження активації режиму годинника
    }

    updateClock(); // Оновлюємо годинник
}

MyClock::MyClock(LiquidCrystal_I2C *lcd, long gmtOffset_sec, int daylightOffset_sec,
                 const char *ntpServer1, const char *ntpServer2)
    : lcd(lcd), gmtOffset_sec(gmtOffset_sec), daylightOffset_sec(daylightOffset_sec),
      ntpServer1(ntpServer1), ntpServer2(ntpServer2) {}

void MyClock::setFont()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Заміна 254 на пробіл (32)
            current_font_digits[i][j] = (square_two_digits[i][j] == 254) ? 32 : square_two_digits[i][j];
        }
    }

    // Створюємо всі 8 користувацьких символів
    memcpy_P(buffer, square_two_00, 8);
    lcd->createChar(0, buffer);
    memcpy_P(buffer, square_two_01, 8);
    lcd->createChar(1, buffer);
    memcpy_P(buffer, square_two_02, 8);
    lcd->createChar(2, buffer);
    memcpy_P(buffer, square_two_03, 8);
    lcd->createChar(3, buffer);
    memcpy_P(buffer, square_two_04, 8);
    lcd->createChar(4, buffer);
    memcpy_P(buffer, square_two_05, 8);
    lcd->createChar(5, buffer);
    memcpy_P(buffer, square_two_06, 8);
    lcd->createChar(6, buffer);
    memcpy_P(buffer, square_two_07, 8);
    lcd->createChar(7, buffer);
}

void MyClock::initClock()
{
    Serial.println("Initializing clock...");

    lcd->init();
    lcd->clear();
    lcd->backlight();
    Serial.println("LCD initialized and cleared");

    setFont();
    Serial.println("Font loaded");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
    Serial.println("NTP configured. Waiting for time sync...");
}

void MyClock::drawDigit(int col, int digit)
{
    lcd->setCursor(col, 0);
    lcd->write(current_font_digits[digit][0]);
    lcd->write(current_font_digits[digit][1]);
    lcd->setCursor(col, 1);
    lcd->write(current_font_digits[digit][2]);
    lcd->write(current_font_digits[digit][3]);
}

void MyClock::displayTime(int hours, int minutes)
{
    lcd->clear();
    drawDigit(0, hours / 10);
    drawDigit(3, hours % 10);
    lcd->setCursor(6, 0);
    lcd->write(':');
    lcd->setCursor(6, 1);
    lcd->write(':');
    drawDigit(7, minutes / 10);
    drawDigit(10, minutes % 10);
}

void MyClock::updateClock()
{
    Serial.println("Updating clock..."); // Лог для початку оновлення годинника

    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        int hours = timeinfo.tm_hour;
        int minutes = timeinfo.tm_min;

        Serial.print("Current time: ");
        Serial.print(hours);
        Serial.print(":");
        Serial.println(minutes);

        displayTime(hours, minutes); // Відображаємо час
    }
    else
    {
        Serial.println("No time available (yet)"); // Лог для випадку, якщо час не отримано
    }
}
