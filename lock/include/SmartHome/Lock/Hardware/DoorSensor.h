#pragma once

#include <cstdint>

namespace SmartHome::Lock::Hardware
{
    class DoorSensor
    {
        private:
            uint8_t _pin;

        public:
            explicit DoorSensor(uint8_t pin);

            void begin();

            bool isOpen() const;
    };
    
} // namespace SmartHome::Lock::Hardware
