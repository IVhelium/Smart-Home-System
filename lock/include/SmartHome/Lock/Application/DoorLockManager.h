#pragma once

#include <cstdint>

namespace SmartHome::Lock
{
    namespace Hardware
    {
        class ServoLock;
        class DoorSensor;
    }
} // namespace SmartHome::Lock


namespace SmartHome::Lock::Application
{
    class DoorLockManager
    {
        private:
            Hardware::ServoLock& _servoLock;
            Hardware::DoorSensor& _doorSensor;
            
            uint32_t _lockDelay;
            uint32_t _unlockTimeout;
            uint32_t _doorClosedAt = 0;
            uint32_t _unlockedAt = 0;

            bool _lockingEnabled = false;
            bool _waitingForDoorOpen = false;
            bool _waitingForLockAfterCLose = false;
            bool _previousDoorOpen = true;

            void lock();
            void unlock();

        public:
            DoorLockManager(
                Hardware::ServoLock& servoLock,
                Hardware::DoorSensor& doorSensor,
                uint32_t lockDelay,
                uint32_t unlockTimeout
            );

            void begin();
            void initializeState(bool lockingEnabled);
            void update(bool lockingEnabled);
            void grantAccess();
            bool isLocked() const;
            bool isDoorOpen() const;
    };
} // namespace SmartHome::Lock::Application
