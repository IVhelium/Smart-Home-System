#include <Arduino.h>
#include "SmartHome/Lock/Application/StartupManager.h"
#include "SmartHome/Lock/Hardware/RFIDReader.h"
#include "SmartHome/Lock/Hardware/Button.h"
#include "SmartHome/Lock/Hardware/Indicators.h"
#include "SmartHome/Lock/Storage/TagStorage.h"

namespace SmartHome::Lock::Application
{
    StartupManager::StartupManager(
        Storage::TagStorage& tagStorage,
        Hardware::RFIDReader& rfidReader,
        Hardware::Button& programButton,
        Hardware::Indicators& indicators,
        uint32_t clearTagsHoldTime
    )
        : _tagStorage(tagStorage),
        _rfidReader(rfidReader),
        _programButton(programButton),
        _indicators(indicators),
        _clearTagsHoldTime(clearTagsHoldTime) 
    {    
    }

    void StartupManager::begin()
    {
        _programButton.begin();
        _tagStorage.begin();
        _rfidReady = _rfidReader.begin();

        if (!_rfidReady)
        {
            Serial.println("RFID INITIALIZATION FAILED");
            _indicators.signalError();
        }

        // Если Program уже зажата при включении, начинаем отсчет CLEAR_TAGS_HOLD_TIME
        if (_programButton.isPressed())
        {
            _bootClearActive = true;
            _bootClearStartedAt = millis();
            _indicators.showProgramming(true);

            Serial.println("Hold PROGRAM button to clear all tags...");

            return;
        }

        // Если конпка при старте не была зажата, пропускаем данный этап
        _bootClearFinished = true;
    }

    StartupResult StartupManager::update()
    {
        _programButton.update();

        // Boot Clear
        if (!_bootClearFinished)
        {
            // Пользователь отпустил раньше времени
            if (!_programButton.isPressed())
            {
                _bootClearActive = false;
                _bootClearFinished = true;
                _indicators.showProgramming(false);

                Serial.println("Clear tags cancelled");
            }
            // Если кнопка все еще удерживается
            else if (_bootClearActive && millis() - _bootClearStartedAt >= _clearTagsHoldTime)
            {
                _tagStorage.clear();

                _bootClearActive = false;
                _bootClearFinished = true;

                Serial.println("ALL RFID TAGS DELETED");

                _indicators.signalTagDeleted();
            }
        }

        // Пока кнопка удерживается - startup не завершен
        if (!_bootClearFinished) return StartupResult::InProgress;
        if (_rfidReady) return StartupResult::Ready;

        return StartupResult::RfidFailed;
    }
        
} // namespace SmartHome::Lock::Application
