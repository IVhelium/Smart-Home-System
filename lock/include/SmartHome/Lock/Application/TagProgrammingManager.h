#pragma once

#include <cstdint>

namespace SmartHome::Lock
{
    namespace Domain
    {
        struct RFIDTag;
    }

    namespace Hardware
    {
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
    class TagProgrammingManager
    {
        private:
            Storage::TagStorage& _tagStorage;
            Hardware::Button& _programButton;
            Hardware::Indicators& _indicators;

            uint32_t _timeout;
            uint32_t _ledInterval;
            uint32_t _startedAt = 0;
            uint32_t _lastBlinkAt = 0;

            bool _active = false;
            bool _ledVisible = true;

            void start();
            void stop();

        public:
            TagProgrammingManager(
                Storage::TagStorage& tagStorage,
                Hardware::Button& programButton,
                Hardware::Indicators& indicators,
                uint32_t timeout,
                uint32_t ledInterval
            );

            void update();
            void handleTag(const Domain::RFIDTag& tag);
            bool isActive() const;
    };
} // namespace SmartHome::Lock::Application
