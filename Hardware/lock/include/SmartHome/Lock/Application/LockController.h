#pragma once

namespace SmartHome::Lock
{
    namespace Domain
    {
        struct RFIDTag;
    }

    namespace Hardware
    {
        class RFIDReader;
        class Indicators;
    }

    namespace Storage
    {
        class TagStorage;
    }

    namespace Application
    {
        class StartupManager;
        class TagProgrammingManager;
        class DoorLockManager;
    }
} // namespace SmartHome::Lock


namespace SmartHome::Lock::Application
{
    class LockController
    {
        private:
            Storage::TagStorage& _tagStorage;
            Hardware::RFIDReader& _rfidReader;
            Hardware::Indicators& _indicators;
            StartupManager& _startupManager;
            TagProgrammingManager& _tagProgrammingManager;
            DoorLockManager& _doorLockManager;

            bool _ready = false;
            bool _rfidAvailable = false;

            void updateRFID();
            void handleAccessTag(const Domain::RFIDTag& tag);
            void syncIndicators();

            static void printUID(const Domain::RFIDTag& tag);

        public:
            LockController(
                Storage::TagStorage& tagStorage,
                Hardware::RFIDReader& rfidReader,
                Hardware::Indicators& indicators,
                StartupManager& startupManager,
                TagProgrammingManager& tagProgrammingManager,
                DoorLockManager& doorLockManager
            );

            void begin();
            void update();

    };
} // namespace SmartHome::Lock::Application
