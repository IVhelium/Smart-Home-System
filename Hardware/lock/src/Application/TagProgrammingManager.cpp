#include <Arduino.h>
#include "SmartHome/Lock/Application/TagProgrammingManager.h"
#include "SmartHome/Lock/Domain/RFIDTag.h"
#include "SmartHome/Lock/Hardware/Button.h"
#include "SmartHome/Lock/Hardware/Indicators.h"
#include "SmartHome/Lock/Storage/TagStorage.h"

namespace SmartHome::Lock::Application
{
    TagProgrammingManager::TagProgrammingManager(
        Storage::TagStorage& tagStorage,
        Hardware::Button& programButton,
        Hardware::Indicators& indicators,
        uint32_t timeout,
        uint32_t ledInterval
    )
        : _tagStorage(tagStorage),
        _programButton(programButton),
        _indicators(indicators),
        _timeout(timeout),
        _ledInterval(ledInterval)
    {
    }

    void TagProgrammingManager::update()
    {
        _programButton.update();

        // Button
        if (_programButton.wasPressed()) start();
        if (!_active) return;      
        
        // Timeout
        if (millis() - _startedAt >= _timeout)
        {
            Serial.println("RFID PROGRAMMING TIMEOUT");

            _indicators.signalProgrammingTimeout();
            stop();

            return;
        }

        // Led Blink
        if (millis() - _lastBlinkAt < _ledInterval) return;

        _lastBlinkAt = millis();
        _ledVisible = !_ledVisible;
        _indicators.showProgramming(_ledVisible);
    }

    void TagProgrammingManager::handleTag(const Domain::RFIDTag& tag)
    {
        if (!_active) return;

        // Если карта уже существует - PROGRAM используется кака удаление
        if (_tagStorage.contains(tag))
        {
            if (_tagStorage.remove(tag))
            {
                Serial.println("RFID TAG DELETED");

                _indicators.signalTagDeleted();
            }
        }
        // Если нет - PROGRAM записывает новую
        else
        {
            if (_tagStorage.add(tag))
            {
                Serial.println("RFID TAG SAVED");

                _indicators.signalTagSaved();
            }
            else
            {
                Serial.println("RFID TAG SAVE FAILED");

                _indicators.signalAccessDenied();
            }
        }

        Serial.print("Stored tags: ");
        Serial.println(_tagStorage.count());

        stop();
    }

    void TagProgrammingManager::start()
    {
        _active = true;
        _startedAt = millis();
        _lastBlinkAt = millis();
        _ledVisible = true;
        _indicators.showProgramming(true);
        _indicators.signalProgrammingStart();

        Serial.println();
        Serial.println("RFID PROGRAMMING MODE");
        Serial.println("Present RFID tag...");
    }

    void TagProgrammingManager::stop()
    {
        _active = false;
    }

    bool TagProgrammingManager::isActive() const
    {
        return _active;
    }

} // namespace SmartHome::Lock::Application
