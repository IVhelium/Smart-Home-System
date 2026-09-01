#include <Arduino.h>
#include "SmartHome/Lock/Application/DoorLockManager.h"
#include "SmartHome/Lock/Hardware/ServoLock.h"
#include "SmartHome/Lock/Hardware/DoorSensor.h"

namespace SmartHome::Lock::Application
{
    DoorLockManager::DoorLockManager(
        Hardware::ServoLock& servoLock,
        Hardware::DoorSensor& doorSensor,
        uint32_t lockDelay,
        uint32_t unlockTimeout
    )
        : _servoLock(servoLock),
        _doorSensor(doorSensor),
        _lockDelay(lockDelay),
        _unlockTimeout(unlockTimeout)
    {
    }

    void DoorLockManager::begin()
    {
        _doorSensor.begin();
        _servoLock.begin();
        _previousDoorOpen = _doorSensor.isOpen();
    }

    void DoorLockManager::initializeState(bool lockingEnabled)
    {
        _lockingEnabled = lockingEnabledl;
        _waitingForDoorOpen = false;
        _waitingForLockAfterCLose = false;

        if (!lockingEnabled || _doorSensor.isOpen())
        {
            unlock(); 
            return;
        }

        lock();
    }

    void DoorLockManager::update(bool lockingEnabled)
    {
        const bool doorOpen = _doorSensor.isOpen();

        // Locking Enabled Changed
        if (!_lockingEnabled && lockingEnabled)
        {
            _lockingEnabled = true;

            if (!doorOpen)
            {
                _doorClosedAt = millis();
                _waitingForLockAfterCLose = true;
            }
        }
        else if (_lockingEnabled && !lockingEnabled)
        {
            _lockingEnabled = false;
            _waitingForDoorOpen = false;
            _waitingForLockAfterCLose = false;

            unlock();
        }

        // Door Opened
        if (doorOpen && !_previousDoorOpen)
        {
            Serial.println("DOOR OPEN");

            _waitingForDoorOpen = false;
            _waitingForLockAfterCLose = false;
        }

        // Door Closed
        if (!doorOpen && _previousDoorOpen)
        {
            Serial.println("DOOR CLOSED");

            _doorClosedAt = millis();

            if (lockingEnabled && !_servoLock.isLocked()) _waitingForLockAfterCLose = true;
        }

        _previousDoorOpen = doorOpen;

        if (!lockingEnabled) return;

        // Auto Lock
        if (_waitingForLockAfterCLose && !doorOpen && !_servoLock.isLocked() && millis() - _doorClosedAt >= _lockDelay)
        {
            Serial.println("AUTO LOCK");

            lock();
            return;
        }

        // Unlock Timeout
        if (_waitingForDoorOpen && !doorOpen && !_servoLock.isLocked() && millis() - _unlockedAt >= _unlockTimeout) // RFID открыл механизм, но дверь за UNLOCK_TIMEOUT не открыли
        {
            Serial.println("Door was not opened -> LOCK");

            lock();
        }
    }

    void DoorLockManager::grantAccess()
    {
        unlock();
        
        _unlockedAt = millis();

        if (!_doorSensor.isOpen())
            _waitingForDoorOpen = true;
        else
            _waitingForDoorOpen = false;

        _waitingForLockAfterCLose = false;
    }

    void DoorLockManager::lock()
    {
        _servoLock.lock();
        _waitingForDoorOpen = false;
        _waitingForLockAfterCLose = false;

        Serial.println("LOCKED");
    }

    void DoorLockManager::unlock()
    {
        _servoLock.unlock();

        Serial.println("UNLOCKED");
    }

    bool DoorLockManager::isLocked() const
    {
        return _servoLock.isLocked();
    }
    
    bool DoorLockManager::isDoorOpen() const
    {
        return _doorSensor.isOpen();
    }

} // namespace SmartHome::Lock::Application
