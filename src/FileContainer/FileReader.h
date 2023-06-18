#pragma once

#include "core/ByteArray/WByteArray.h"
#include "FileInstance.h"
#include <memory>

class FileReader: public FileInstance {
    std::shared_ptr<const WByteArray> _data;
public:
    FileReader ();
    FileReader (std::shared_ptr<const WByteArray> &data);
    FileReader (std::shared_ptr<const WByteArray> &&data);
    ~FileReader() = default;

    [[nodiscard]]
    bool isOk() const;

    size_t readRaw(void *to, size_t size) const;
};

size_t FileReader::readRaw(void *to, size_t size) const
{
    if (size > _data->size()) {
        memcpy(to, _data->constData(), size);
        return size;
    }

    return _data->size();
}
