#pragma once

#include <cstdint>
#include <MFRC522.h>
#include "../lock/include/SmartHome/Lock/Domain/RFIDTag.h"

namespace SmartHome::Lock::Hardware
{
    class RFIDReader
    {
        private:
            uint8_t _sdaPin;
            uint8_t _sckPin;
            uint8_t _mosiPin;
            uint8_t _misoPin;
            uint8_t _rstPin;

            MFRC522 _reader;

        public:
            RFIDReader(
                uint8_t sdaPin,
                uint8_t sckPin,
                uint8_t mosiPin,
                uint8_t misoPin,
                uint8_t rstPin
            );
            
            bool begin();
            bool tryRead(Domain::RFIDTag& tag);
    };
} // namespace SmartHome::Lock::Hardware
