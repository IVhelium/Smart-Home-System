#pragma once 

#include <cstdint>

namespace SmartHome::Lock::Config::Settings
{
    // RFID
    constexpr uint8_t MAX_TAGS     = 10;
    constexpr uint8_t MAX_UID_SIZE = 10;

    constexpr uint32_t PROGRAM_TIMEOUT_MS      = 10000; // Ожидание метки после включения режима программирования
    constexpr uint32_t PROGRAM_LED_INTERVAL_MS = 400;   // Интервал мигания светодиода в режиме программирования

    // Servo
    constexpr uint8_t SERVO_LOCK_ANGLE    = 170;
    constexpr uint8_t SERVO_UNLOCK_ANGLE  = 10;

    constexpr uint16_t SERVO_FREQUENCY_HZ = 50;

    constexpr uint16_t SERVO_MIN_PULSE_US = 500;
    constexpr uint16_t SERVO_MAX_PULSE_US = 2400;

    // Buttons
    constexpr uint32_t BUTTON_DEBOUNCE_MS = 50;
    constexpr uint32_t CLEAR_TAGS_HOLD_MS = 6000; // Удержание при включении для удаления всех RFID меток

    // Lock
    constexpr uint32_t LOCK_DELAY_MS     = 2000;  // Сколько ждать после физического закрытия двери
    constexpr uint32_t UNLOCK_TIMEOUT_MS = 7000;  // Открыли RFID, но дверь физически не открыли

} // SmartHome::Lock::Config::Settings