#include <Arduino.h>
#include "SmartHome/Lock/Config/Pins.h"
#include "SmartHome/Lock/Config/Settings.h"
#include "SmartHome/Lock/Hardware/RFIDReader.h"
#include "SmartHome/Lock/Hardware/ServoLock.h"
#include "SmartHome/Lock/Hardware/Button.h"
#include "SmartHome/Lock/Hardware/DoorSensor.h"
#include "SmartHome/Lock/Hardware/Indicators.h"
#include "SmartHome/Lock/Storage/TagStorage.h"
#include "SmartHome/Lock/Application/StartupManager.h"
#include "SmartHome/Lock/Application/TagProgrammingManager.h"
#include "SmartHome/Lock/Application/DoorLockManager.h"
#include "SmartHome/Lock/Application/LockController.h"

namespace Lock = SmartHome::Lock;

#pragma region Hardware

// RFID
Lock::Hardware::RFIDReader rfidReader(
    Lock::Config::Pins::RFID_SS_PIN,
    Lock::Config::Pins::RFID_SCK_PIN,
    Lock::Config::Pins::RFID_MOSI_PIN,
    Lock::Config::Pins::RFID_MISO_PIN,
    Lock::Config::Pins::RFID_RST_PIN
);

// Servo
Lock::Hardware::ServoLock servoLock(
    Lock::Config::Pins::SERVO_PIN,
    Lock::Config::Settings::SERVO_LOCK_ANGLE,
    Lock::Config::Settings::SERVO_UNLOCK_ANGLE,
    Lock::Config::Settings::SERVO_FREQUENCY_HZ,
    Lock::Config::Settings::SERVO_MIN_PULSE_US,
    Lock::Config::Settings::SERVO_MAX_PULSE_US
);

// PROGRAM button
Lock::Hardware::Button programButton(
    Lock::Config::Pins::PROGRAM_BUTTON_PIN,
    Lock::Config::Settings::BUTTON_DEBOUNCE_MS
);

// Door Sensor
Lock::Hardware::DoorSensor doorSensor(
    Lock::Config::Pins::DOOR_BUTTON_PIN
);

// Indicators
Lock::Hardware::Indicators indicators(
    Lock::Config::Pins::BUZZER_PIN,
    Lock::Config::Pins::RED_LED_PIN,
    Lock::Config::Pins::GREEN_LED_PIN,
    Lock::Config::Settings::RGB_COMMON_ANODE
);

#pragma endregion

// Storage
Lock::Storage::TagStorage tagStorage;


#pragma region Application

// Startup
Lock::Application::StartupManager startupManager(
    tagStorage,
    rfidReader,
    programButton,
    indicators,
    Lock::Config::Settings::CLEAR_TAGS_HOLD_MS
);

// RFID programming
Lock::Application::TagProgrammingManager tagProgrammingManager(
    tagStorage,
    programButton,
    indicators,
    Lock::Config::Settings::PROGRAM_TIMEOUT_MS,
    Lock::Config::Settings::PROGRAM_LED_INTERVAL_MS
);

// Door + servo
Lock::Application::DoorLockManager doorLockManager(
    servoLock,
    doorSensor,
    Lock::Config::Settings::LOCK_DELAY_MS,
    Lock::Config::Settings::UNLOCK_TIMEOUT_MS
);

// Lock Controller
Lock::Application::LockController lockController(
    tagStorage,
    rfidReader,
    indicators,
    startupManager,
    tagProgrammingManager,
    doorLockManager
);

#pragma endregion


void setup()
{
    Serial.begin(115200);
    lockController.begin();
}

void loop()
{
    lockController.update();
}