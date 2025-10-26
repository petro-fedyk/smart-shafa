# ESP32 Security & Alarm System

This project implements a **home security system** using ESP32 with keypad access, LCD display, Wi-Fi connectivity, OTA updates, and an alarm system. It is designed for controlling access and monitoring alarm status.

---

## Features

- **Keypad input** for secure access
- **LCD display** (16x2) for user interface
- **Wi-Fi connectivity** for remote management
- **OTA (Over-the-Air) updates**
- **Alarm system** with buzzer and transistor-controlled lock
- Configurable **NTP clock** for timekeeping
- Supports integration with storage and optional MQTT

---

## Hardware Requirements

- ESP32 board
- 16x2 I2C LCD
- 4x4 Keypad
- Transistor for lock control
- Buzzer
- Optional: external sensors or alarm components

---

## Installation

1. Clone the repository:

```bash
git clone https://github.com/yourusername/esp32-security-system.git
cd esp32-security-system
