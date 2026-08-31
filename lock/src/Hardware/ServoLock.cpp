#include "../lock/include/SmartHome/Lock/Hardware/ServoLock.h"

namespace SmartHome::Lock::Hardware
{
    ServoLock::ServoLock(
        uint8_t pin,
        uint8_t lockedAngle,
        uint8_t unlockedAngle,
        uint16_t frequency,
        uint16_t minPulse,
        uint16_t maxPulse
    )
        : _pin(pin),
        _lockedAngle(lockedAngle),
        _unlockedAngle(unlockedAngle),
        _frequency(frequency),
        _minPulse(minPulse),
        _maxPulse(maxPulse)
    {
    }

    void ServoLock::begin()
    {
        _servo.setPeriodHertz(_frequency);
        _servo.attach(
            _pin,
            _minPulse,
            _maxPulse
        );
    }

    void ServoLock::lock()
    {
        if (_locked) return;

        _servo.write(_lockedAngle);
        _locked = true;
    }

    void ServoLock::unlock()
    {
        _servo.write(_unlockedAngle);
        _locked = false;
    }

    bool ServoLock::isLocked() const
    {
        return _locked;
    }
} // namespace SmartHome::Lock::Hardware
