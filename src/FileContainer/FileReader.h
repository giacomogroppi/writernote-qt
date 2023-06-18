#pragma once

#include "core/ByteArray/WByteArray.h"
#include "FileInstance.h"
#include "core/pointer/SharedPtr.h"
#include <memory>

class FileReader: public FileInstance {
    SharedPtr<const WByteArray> _data;
public:
    FileReader ();
    FileReader (const SharedPtr<const WByteArray> &data);
    FileReader (SharedPtr<const WByteArray> &&data);
    ~FileReader() = default;

    [[nodiscard]]
    bool isOk() const;

    size_t readRaw(void *to, size_t size) const;
};

inline size_t FileReader::readRaw(void *to, size_t size) const
{
    if (size > _data->size()) {
        memcpy(to, _data->constData(), size);
        return size;
    }

    return _data->size();
}
