#include <cstddef>
#include <cstring>
#include "SmartHome/Lock/Storage/TagStorage.h"

namespace SmartHome::Lock::Storage
{
    namespace
    {
        constexpr const char* NVS_NAMESPACE = "rfidlock";
        constexpr const char* COUNT_KEY = "count";
        constexpr const char* TAGS_KEY = "tags";
    }

    void TagStorage::begin()
    {
        _preferences.begin(NVS_NAMESPACE, false);
        load();
    }

    void TagStorage::load()
    {
        std::memset(_tags, 0, sizeof(_tags));   // Очищаем память перед чтением

        _count = _preferences.getUChar(COUNT_KEY, 0);

        if (_count > Config::Settings::MAX_TAGS) _count = 0;

        const std::size_t storedSize = _preferences.getBytesLength(TAGS_KEY);

        // Размер сохраненного массива должен совпадать с размером текущей структуры
        if (storedSize != sizeof(_tags))
        {
            _count = 0;
            return;
        }

        _preferences.getBytes(TAGS_KEY, _tags, sizeof(_tags));
    }

    void TagStorage::save()
    {
        _preferences.putUChar(COUNT_KEY, _count);
        _preferences.putBytes(TAGS_KEY, _tags, sizeof(_tags));
    }

    int TagStorage::findIndex(const Domain::RFIDTag& tag) const
    {
        for (uint8_t i = 0; i < _count; ++i)
        {
            const Domain::RFIDTag& stored = _tags[i];

            if (stored.size != tag.size) continue;

            bool equal = true;

            for (uint8_t j = 0; j < tag.size; ++j)
            {
                if (stored.uid[j] != tag.uid[j])
                {
                    equal = false;
                    break;
                }
            }
            
            if (equal) return i;
        }

        return -1; // Если не найдено
    }

    bool TagStorage::contains(const Domain::RFIDTag& tag) const
    {
        return findIndex(tag) >= 0;
    }

    bool TagStorage::add(const Domain::RFIDTag& tag)
    {
        if (contains(tag)) return false;
        if (_count >= Config::Settings::MAX_TAGS) return false;

        _tags[_count] = tag;
        ++_count;

        save();

        return true;
    }

    bool TagStorage::remove(const Domain::RFIDTag& tag)
    {
        const int index = findIndex(tag);

        if (index < 0) return false;

        // После удаления сдвигаем оставшиеся элементы влево
        for (uint8_t i = index; i < _count - 1; ++i)
        {
            _tags[i] = _tags[i + 1];
        }

        std::memset(&_tags[_count - 1], 0, sizeof(Domain::RFIDTag));
        --_count;

        save();

        return true;
    }

    void TagStorage::clear()
    {
        std::memset(_tags, 0, sizeof(_tags));

        _count = 0;
        _preferences.clear();   // Удаляем ключи текущего неймспейса

        save();
    }

    uint8_t TagStorage::count() const
    {
        return _count;
    }

} // namespace SmartHome::Lock::Storage
