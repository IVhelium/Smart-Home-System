#include <Arduino.h>
#include "SmartHome/Lock/Application/LockController.h"
#include "SmartHome/Lock/Application/StartupManager.h"
#include "SmartHome/Lock/Application/TagProgrammingManager.h"
#include "SmartHome/Lock/Application/DoorLockManager.h"
#include "SmartHome/Lock/Domain/RFIDTag.h"
#include "SmartHome/Lock/Hardware/RFIDReader.h"
#include "SmartHome/Lock/Hardware/Indicators.h"
#include "SmartHome/Lock/Storage/TagStorage.h"

namespace SmartHome::Lock::Application
{
    LockController::LockController(
        Storage::TagStorage& tagStorage,
        Hardware::RFIDReader& rfidReader,
        Hardware::Indicators& indicators,
        StartupManager& startupManager,
        TagProgrammingManager& tagProgrammingManager,
        DoorLockManager& doorLockManager
    )
        : _tagStorage(tagStorage),
        _rfidReader(rfidReader),
        _indicators(indicators),
        _startupManager(startupManager),
        _tagProgrammingManager(tagProgrammingManager),
        _doorLockManager(doorLockManager)
    {
    }

    void LockController::begin()
    {
        Serial.println();
        Serial.println("=======================");
        Serial.println("ESP32 SMART LOCK");
        Serial.println("=======================");

        _indicators.begin();
        _doorLockManager.begin();
        _startupManager.begin();
    }

    void LockController::update()
    {
        _indicators.update();

        // Startup
        if (!_ready)
        {
            const StartupResult result = _startupManager.update();

            if (result == StartupResult::InProgress) return;

            _ready = true;
            _rfidAvailable = result == StartupResult::Ready;
            const bool lockingEnabled = _rfidAvailable && _tagStorage.count() > 0;
            _doorLockManager.initializeState(lockingEnabled);

            syncIndicators();

            if (!_rfidAvailable) Serial.println("RFID unavailable -> lock disabled");

            Serial.println("SYSTEM READY");

            return;
        }

        // Program Mode
        if (_rfidAvailable) _tagProgrammingManager.update();

        // Door
        const bool lockingEnabled = _rfidAvailable && _tagStorage.count() > 0;
        _doorLockManager.update(lockingEnabled);

        syncIndicators();

        // RFID
        if (_rfidAvailable) updateRFID();
    }

    void LockController::updateRFID()
    {
        Domain::RFIDTag tag;

        if (!_rfidReader.tryRead(tag)) return;

        printUID(tag);

        // Program Mode
        if (_tagProgrammingManager.isActive())
        {
            _tagProgrammingManager.handleTag(tag);

            syncIndicators();
            return;
        }

        // Normal Access
        handleAccessTag(tag);
    }

    void LockController::handleAccessTag(const Domain::RFIDTag& tag)
    {
        if (!_tagStorage.contains(tag))
        {
            Serial.println("ACCESS DENIED");

            _indicators.signalAccessDenied();

            return;
        }

        Serial.println("ACCESS GRANTED");

        _indicators.signalAccessGranted();
        _doorLockManager.grantAccess();

        syncIndicators();
    }

    void LockController::syncIndicators()
    {
        if (_tagProgrammingManager.isActive()) return;

        if (_doorLockManager.isLocked())
            _indicators.showLocked();
        else
            _indicators.showUnlocked();
    }

    void LockController::printUID(const Domain::RFIDTag& tag)
    {
        Serial.print("UID: ");

        for (uint8_t i = 0; i < tag.size; ++i)
        {
            if (tag.uid[i] < 0x10) Serial.print("0");

            Serial.print(tag.uid[i], HEX);

            if (i < tag.size - 1) Serial.print(":");
        }

        Serial.println();
    }

} // namespace SmartHome::Lcok::Application
