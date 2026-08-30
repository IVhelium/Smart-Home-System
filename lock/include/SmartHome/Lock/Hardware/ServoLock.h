#pragma once

#include <cstdint>
#include <ESP32Servo.h>

namespace SmartHome::Lock::Hardware
{
    class ServoLock
    {
        private:
            Servo _servo;
            uint8_t _pin;
            uint8_t _lockedAngle;
            uint8_t _unlockedAngle;
            uint16_t _frequency;
            uint16_t _minPulse;
            uint16_t _maxPulse;
            bool _locked = false;

        public : 
            ServoLock(
                uint8_t pin,
                uint8_t lockedAngle,
                uint8_t unlockedAngle,
                uint16_t frequency,
                uint16_t minPulse,
                uint16_t maxPulse
            );

            void begin();
            void lock();
            void unlock();

            bool isLocked() const;
    };
    
} // namespace SmartHome::Lock::Hardware
