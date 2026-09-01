#pragma once

#include <cstdint>

namespace SmartHome::Lock
{
    namespace Hardware
    {
        class RFIDReader;
        class Button;
        class Indicators;
    }

    namespace Storage
    {
        class TagStorage;
    }
} // namespace SmartHome::Lock


namespace SmartHome::Lock::Application
{
    /*
        Результат запуска системы

        - InProgress:
        startup еще продолжается

        - Ready:
        RFID успешно инициализирован

        - RfidFailed:
        система запустилась, но RFID неисправен
    */
    enum class StartupResult : uint8_t
    {
        InProgress,
        Ready,
        RfidFailed
    };

    class StartupManager
    {
        private:
            // Dependencies
            Storage::TagStorage& _tagStorage;
            Hardware::RFIDReader& _rfidReader;
            Hardware::Button& _programButton;
            Hardware::Indicators& _indicators;

            // Startup State
            bool _rfidReady = false;
            bool _startupFinished = false;
            bool _bootClearFinished = false;
            bool _bootClearActive = false;

            uint32_t _bootClearStartedAt = 0;
            uint32_t _clearTagsHoldTime;

        public:
            StartupManager(
                Storage::TagStorage& tagStorage,
                Hardware::RFIDReader& rfidReader,
                Hardware::Button& programButton,
                Hardware::Indicators& indicators,
                uint32_t clearTagsHoldTime
            );

            void begin();
            StartupResult update();
    };

} // namespace SmartHome::Lock::Application
