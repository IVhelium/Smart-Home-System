#include <Arduino.h>
#include "../lock/include/SmartHome/Lock/Hardware/Button.h"

namespace SmartHome::Lock::Hardware
{
    Button::Button(
        uint8_t pin,
        uint32_t debounceMs
    ) 
        : _pin(pin),
        _debounceMs(debounceMs)
    {
    }

    void Button::begin()
    {
        pinMode(_pin, INPUT_PULLUP);

        const bool state = digitalRead(_pin);

        _lastReading = state;
        _stableState = state;
        _lastChangedAt = millis();
    }

    void Button::update()
    {
        const bool reading = digitalRead(_pin);

        if (reading != _lastReading)
        {
            _lastReading = reading;
            _lastChangedAt = millis();
        }

        if (millis() - _lastChangedAt < _debounceMs) return;
        if (reading == _stableState) return;

        _stableState = reading;

        if (_stableState == LOW) _pressedEvent = true;
    }

    bool Button::isPressed() const 
    {
        return _stableState == LOW;
    }

    bool Button::wasPressed()
    {
        if (!_pressedEvent) return false;

        _pressedEvent = false;

        return true;
    }
    
} // namespace SmartHome::Lock::Hardware
