#pragma once

#include <cstdint>

namespace SmartHome::Lock::Hardware
{
    class Button
    {
        private:
            uint8_t _pin;
            uint32_t _debounceMs;
            uint32_t _lastChangedAt = 0;
            bool _lastReading = true;
            bool _stableState = true;
            bool _pressedEvent = false;
    };
} // namespace SmartHome::Lock::Hardware
