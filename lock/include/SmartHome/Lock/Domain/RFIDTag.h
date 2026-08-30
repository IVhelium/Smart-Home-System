#pragma once

#include <cstdint>
#include "../lock/include/SmartHome/Lock/Config/Settings.h"

namespace SmartHome::Lock::Domain
{
    struct RFIDTag
    {
        uint8_t size = 0;
        uint8_t uid[Config::Settings::MAX_UID_SIZE]{};
    };
    
} // namespace SmartHome::Lock::Domain
