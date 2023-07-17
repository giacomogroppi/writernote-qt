#include "WVariant.h"
#include "FileContainer/MemWritable.h"

WVariant::WVariant(const void *raw, int size)
    : _data(
            static_cast<const char *>(raw),
            size
    )
{

}

WVariant::WVariant(int data)
    : _data()
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
