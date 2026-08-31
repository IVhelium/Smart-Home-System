#include <Arduino.h>
#include "../lock/include/SmartHome/Lock/Hardware/Indicators.h"

namespace SmartHome::Lock::Hardware
{
    Indicators::Indicators(
        uint8_t buzzerPin,
        uint8_t redPin,
        uint8_t greenPin,
        bool commonAnode
    )
        : _buzzerPin(buzzerPin),
        _redPin(redPin),
        _greenPin(greenPin),
        _commonAnode(commonAnode)
    {
    }

    void Indicators::begin()
    {
        pinMode(_buzzerPin, OUTPUT);
        pinMode(_redPin, OUTPUT);
        pinMode(_greenPin, OUTPUT);

        noTone(_buzzerPin);
        applyBaseState();
    }

    void Indicators::update()
    {
        if (_signal == Signal::None) return;

        Step currentStep;

        if (!getStep(_stepIndex, currentStep))
        {
            _signal = Signal::None;

            noTone(_buzzerPin);
            applyBaseState();

            return;
        }

        if (millis() - _stepStartedAt < currentStep.durationMs) return;

        ++_stepIndex;

        Step nextStep;

        if (!getStep(_stepIndex, nextStep))
        {
            _signal = Signal::None;

            noTone(_buzzerPin);
            applyBaseState();

            return;
        }

        _stepStartedAt = millis();

        applyStep(nextStep);
    }

    void Indicators::showLocked()
    {
        _baseState = BaseState::Locked;

        if (_signal == Signal::None) applyBaseState();
    }

    void Indicators::showUnlocked()
    {
        _baseState = BaseState::Unlocked;

        if (_signal == Signal::None) applyBaseState();
    }

    void Indicators::showProgramming(bool visible)
    {
        _baseState = visible ? BaseState::ProgrammingOn : BaseState::ProgrammingOff;

        if (_signal == Signal::None) applyBaseState();
    }

    void Indicators::signalAccessGranted()
    {
        startSignal(Signal::AccessGranted);
    }

    void Indicators::signalAccessDenied()
    {
        startSignal(Signal::AccessDenied);
    }

    void Indicators::signalProgrammingStart()
    {
        startSignal(Signal::ProgrammingStart);
    }

    void Indicators::signalProgrammingTimeout()
    {
        startSignal(Signal::ProgrammingTimeout);
    }

    void Indicators::signalTagSaved()
    {
        startSignal(Signal::TagSaved);
    }

    void Indicators::signalTagDeleted()
    {
        startSignal(Signal::TagDeleted);
    }

    void Indicators::signalError()
    {
        startSignal(Signal::Error);
    }

    void Indicators::startSignal(Signal signal)
    {
        _signal = signal;
        _stepIndex = 0;
        _stepStartedAt = millis();

        Step step;

        if (getStep(0, step)) applyStep(step);
    }

    bool Indicators::getStep(
        uint8_t index,
        Step& step
    ) const
    {
        switch (_signal)
        {
            case Signal::AccessGranted:
            {
                static constexpr Step steps[] = 
                {
                    { false, true, 1500, 150 }
                };

                if (index >= 1) return false;

                step = steps[index];

                return true;
            }

            case Signal::AccessDenied:
            {
                static constexpr Step steps[] =
                {
                    { true, false, 300, 200 },
                    { false, false, 0, 100 },
                    { true, false, 300, 200 }
                };

                if (index >= 3) return false;

                step = steps[index];

                return true;
            }

            case Signal::ProgrammingStart:
            {
                static constexpr Step steps[] = 
                {
                    { false, true, 1800, 100 }
                };

                if (index >= 1) return false;

                step = steps[index];

                return true;
            }

            case Signal::ProgrammingTimeout:
            {
                static constexpr Step steps[] = 
                {
                    { false, false, 500, 100 }
                };

                if (index >= 1) return false;

                step = steps[index];

                return true;
            }

            case Signal::TagSaved:
            {
                static constexpr Step steps[] = 
                {
                    { false, true, 1300, 120 },
                    { false, false, 0, 100 },
                    { false, true, 1300, 120 }
                };

                if (index >= 3) return false;

                step = steps[index];

                return true;
            }

            case Signal::TagDeleted:
            {
                static constexpr Step steps[] = 
                {
                    { true, false, 800, 120 },
                    { false, false, 0, 80 },
                    { true, false, 800, 120 },
                    { false, false, 0, 80 },
                    { true, false, 800, 120 },
                };

                if (index >= 5) return false;

                step = steps[index];

                return true;
            }

            case Signal::Error:
            {
                if (index >= 20) return false;

                const bool visible = index % 2 == 0;

                step = { visible, false, 0, 80 };

                return true;
            }
            
            case Signal::None:
            default:
                break;
        }
    }

    void Indicators::applyStep(const Step& step)
    {
        setColor(step.red, step.green);

        if (step.frequency == 0) 
            noTone(_buzzerPin);
        else 
            tone(_buzzerPin, step.frequency);
    }

    void Indicators::applyBaseState()
    {
        switch (_baseState)
        {
            case BaseState::Locked:
                setColor(true, false);
                break;
            
            case BaseState::Unlocked:
            case BaseState::ProgrammingOn:
                setColor(false, true);
                break;

            case BaseState::ProgrammingOff:
            case BaseState::Off:
                setColor(false, false);
                break;
        }
    }

    void Indicators::setLed(
        uint8_t pin,
        bool enabled
    )
    {
        digitalWrite(pin, _commonAnode ? (enabled ? LOW : HIGH) : (enabled ? HIGH : LOW));
    }

    void Indicators::setColor(
        bool red,
        bool green
    )
    {
        setLed(_redPin, red);
        setLed(_greenPin, green);
    }

} // namespace SmartHome::Lock::Hardware
