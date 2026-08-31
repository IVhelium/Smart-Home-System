#include <Arduino.h>
#include <SPI.h>
#include <cstring>
#include "../lock/include/SmartHome/Lock/Hardware/RFIDReader.h"
#include "../lock/include/SmartHome/Lock/Config/Settings.h"

namespace SmartHome::Lock::Hardware
{
    RFIDReader::RFIDReader(
        uint8_t sdaPin, 
        uint8_t sckPin, 
        uint8_t mosiPin, 
        uint8_t misoPin, 
        uint8_t rstPin
    ) 
        : _sdaPin(sdaPin),
        _sckPin(sckPin),
        _mosiPin(mosiPin),
        _misoPin(misoPin),
        _rstPin(rstPin),
        _reader(sdaPin, rstPin)
    {
    }

    bool RFIDReader::begin()
    {
        Serial.println("Starting RC522...");

        SPI.begin(
            _sdaPin,
            _sckPin,
            _misoPin,
            _mosiPin
        );

        // MFRC522 Config
        _reader.PCD_Init();
        _reader.PCD_SetAntennaGain(MFRC522::RxGain_max);
        _reader.PCD_AntennaOn();

        const uint8_t version = _reader.PCD_ReadRegister(MFRC522::VersionReg);
        const uint8_t txControl = _reader.PCD_ReadRegister(MFRC522::TxControlReg);

        // RC522 Version
        Serial.print("RC522 version: 0x");
        Serial.println(version, HEX);
        // RC522 TxControl
        Serial.print("RC522 TxControl: 0x");
        Serial.println(txControl, HEX);

        if (version == 0x12) Serial.println("RC522 clone 0x12");
        
        return (txControl & 0x03) == 0x03;
    }

    bool RFIDReader::tryRead(Domain::RFIDTag &tag)
    {
        if (!_reader.PICC_IsNewCardPresent()) return false;
        if (!_reader.PICC_ReadCardSerial()) return false;

        if (_reader.uid.size > Config::Settings::MAX_UID_SIZE) 
        {
            _reader.PICC_HaltA(); // Не дает бесконечно считивать одну метку
            _reader.PCD_StopCrypto1();

            return false;
        }

        tag.size = _reader.uid.size;

        std::memset(tag.uid, 0, Config::Settings::MAX_UID_SIZE); // Очищаем массив перед записью
        std::memcpy(tag.uid, _reader.uid.uidByte, tag.size);     // Записываем новую метку в пустой массив

        _reader.PICC_HaltA();
        _reader.PCD_StopCrypto1();

        return true;
    }

} // namespace SmartHome::Lock::Hardware
