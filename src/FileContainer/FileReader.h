#pragma once

#include "core/ByteArray/WByteArray.h"
#include "FileInstance.h"
#include "core/pointer/SharedPtr.h"
#include <memory>

class FileReader final: public FileInstance, public ReadableAbstract
{
    SharedPtr<const WByteArray> _data;
    mutable size_t _delta;
public:
    FileReader ();
    FileReader (const SharedPtr<const WByteArray> &data);
    FileReader (SharedPtr<const WByteArray> &&data);
    ~FileReader() = default;

    [[nodiscard]]
    bool isOk() const;

    auto read(void *to, size_t size) const -> int final;
};

inline auto FileReader::read(void *to, size_t size) const -> int
{
    if (_delta + size > static_cast<unsigned long>(_data->size()))
        return -1;

    WCommonScript::WMemcpy(to, _data->constData() + _delta, size);
    _delta += size;

    return 0;
}
