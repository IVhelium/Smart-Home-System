#pragma once

#include <cstdint>
#include <Preferences.h>
#include "SmartHome/Lock/Domain/RFIDTag.h"
#include "SmartHome/Lock/Config/Settings.h"

namespace SmartHome::Lock::Storage
{
    class TagStorage
    {
        private:
            Preferences _preferences;
            Domain::RFIDTag _tags[Config::Settings::MAX_TAGS]{};

            uint8_t _count = 0;

            int findIndex(const Domain::RFIDTag& tag) const;

            void load();
            void save();

        public:
            void begin();
            void clear();
            bool contains(const Domain::RFIDTag& tag) const;
            bool add(const Domain::RFIDTag& tag);
            bool remove(const Domain::RFIDTag& tag);
            uint8_t count() const;
    };
} // namespace SmartHome::Lock::Storage
