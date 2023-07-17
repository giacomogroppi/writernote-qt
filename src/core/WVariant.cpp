#include "WVariant.h"
#include "FileContainer/MemWritable.h"

WVariant::WVariant(const void *raw, int size)
    : _data(
            static_cast<const char *>(raw),
            size
    )
    , _version()
{

}

WVariant::WVariant(int data)
    : _data()
    , _version()
{
    _data.reserve(sizeof(data));
    MemWritable m;
    m.write(&data, sizeof(data));

    m.merge([this](const void *data, size_t size) -> int {
        this->_data.append(static_cast<const char *>(data), size);
        return 0;
    });
}

WVariant::WVariant(const WByteArray &data)
    : _data()
    , _version()
{
    MemWritable writable;
    WByteArray::write(writable, data);
    writable.merge([this](const void *d, size_t size) {
        this->_data.append(static_cast<const char *>(d), size);
        return 0;
    });
}

auto WVariant::toInt() -> int
{
    int result;
    MemReadable readable;
    readable.setData(_data.constData(), _data.size());

    if (readable.read(&result, sizeof (result)) < 0) {
        WDebug(true, "Error reading" << _data);
        return -1;
    }
    return result;
}

auto WVariant::create(VersionFileController version, WByteArray d) -> WVariant
{
    WVariant result;

    result._data = std::move(d);
    result._version = std::move(version);

    return result;
}

auto WVariant::toString() -> WString
{
    const auto [r, d] = this->loadFromClass<WString>();
    if (r < 0)
        return {};
    return d;
}
