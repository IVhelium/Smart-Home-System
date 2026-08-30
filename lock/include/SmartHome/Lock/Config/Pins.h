#pragma once

#include <cstdint>

namespace SmartHome::Lock::Config::Pins
{
    // RC522
    constexpr uint8_t RFID_SDA_PIN  = 5;
    constexpr uint8_t RFID_SCK_PIN  = 18;
    constexpr uint8_t RFID_MOSI_PIN = 23;
    constexpr uint8_t RFID_MISO_PIN = 19;
    constexpr uint8_t RFID_RST_PIN  = 32;

    // Servo
    constexpr uint8_t SERVO_PIN = 25;

    // Buzzer
    constexpr uint8_t BUZZER_PIN = 16;

    // Buttons
    constexpr uint8_t PROGRAM_BUTTON_PIN = 26;
    constexpr uint8_t DOOR_BUTTON_PIN    = 33;
    
    // LED
    constexpr uint8_t RED_LED_PIN   = 13;
    constexpr uint8_t GREEN_LED_PIN = 14;

} // namespace SmartHome::Lock::Config::Pins
