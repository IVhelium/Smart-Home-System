#include <Arduino.h>
#include "../lock/include/SmartHome/Lock/Hardware/DoorSensor.h"

namespace SmartHome::Lock::Hardware
{
    DoorSensor::DoorSensor(uint8_t pin) : _pin(pin) 
    {
    }

    void DoorSensor::begin()
    {
        pinMode(_pin, INPUT_PULLUP);
    }

    bool DoorSensor::isOpen() const
    {
        // HIGH -> Концевик разомкнут -> Дверь открыта
        return digitalRead(_pin) == HIGH;
    }

} // namespace SmartHome::Lock::Hardware
