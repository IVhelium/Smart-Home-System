#pragma once

#include <cstdint>

namespace SmartHome::Lock::Hardware
{
    class Indicators
    {
        private:
            enum class BaseState : uint8_t
            {
                Off,
                Locked,
                Unlocked,
                ProgrammingOn,
                ProgrammingOff
            };

            enum class Signal : uint8_t
            {
                None,
                AccessGranted,
                AccessDenied,
                ProgrammingStart,
                ProgrammingTimeout,
                TagSaved,
                TagDeleted,
                Error
            };

            struct Step
            {
                bool red;
                bool green;
                uint16_t frequency;
                uint16_t durationMs;
            };
            
            uint8_t _buzzerPin;
            uint8_t _redPin;
            uint8_t _greenPin;
            uint8_t _stepIndex = 0;
            uint32_t _stepStartedAt = 0; 
            bool _commonAnode;

            BaseState _baseState = BaseState::Off;
            Signal _signal = Signal::None;

            void startSignal(Signal signal);
            void applyStep(const Step& step);
            void applyBaseState();
            void setLed(uint8_t pin, bool enebled);
            void setColor(bool red, bool green);
            bool getStep(uint8_t index, Step &step) const;

        public:
            Indicators(
                uint8_t buzzerPin,
                uint8_t redPin,
                uint8_t greenPin,
                bool commonAnode
            );

            void begin();
            void update(); // Вызывается в loop
            void showLocked();
            void showUnlocked();
            void showProgramming(bool visible);
            void signalAccessGranted();
            void signalAccessDenied();
            void signalProgrammingStart();
            void signalProgrammingTimeout();
            void signalTagSaved();
            void signalTagDeleted();
            void signalError();
    };
    
} // namespace SmartHome::Lock::Hardware
