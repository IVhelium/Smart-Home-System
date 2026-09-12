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

        public:
            Button(
                uint8_t pin,
                uint32_t debounceMs
            );

            void begin();
            void update();

            bool isPressed() const;
            bool wasPressed();
    };
} // namespace SmartHome::Lock::Hardware
