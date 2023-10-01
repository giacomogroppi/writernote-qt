#include "WByteArray.h"
#include "utils/WCommonScript.h"
#include "core/WPair.h"

#ifndef USE_QT
WByteArray::WByteArray()
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
}

WByteArray::WByteArray(const char *data, Size size)
{
    if (size == 0)
        size = strlen(data);

    _data = (char *) malloc (size);
    memcpy(_data, data, size);
    _size = size;
    _reserved = 0;
}

WByteArray::~WByteArray()
{
    free(this->_data);
    this->_data = nullptr;
    this->_size = 0;
}
#endif

auto WByteArray::load(
        const VersionFileController &versionController,
        ReadableAbstract &readable) -> WPair<int, WByteArray>
{
    WByteArray result;

    if (versionController.getVersionWByteArray() != 0)
        return {-1, {}};

    TypeofSize size;
    if (readable.read(size) < 0)
        return {-1, {}};

    char d[size];
    if (readable.read(d, size) < 0)
        return {-1, {}};

    result = WByteArray(d, size);

    return {0, result};
}

auto WByteArray::write(WritableAbstract &writable, const WByteArray &object) -> int
{
    TypeofSize size = object.size();

    if (writable.write(size) < 0)
        return -1;

    if (writable.write(object.constData(), size) < 0)
        return -1;
    return 0;
}

auto WByteArray::loadPtr(
        const VersionFileController &versionFile,
        ReadableAbstract &readableAbstract
) -> WPair<int, WByteArray *>
{
    auto [res, data] = WByteArray::load (versionFile, readableAbstract);
    if (res < 0)
        return {-1, nullptr};

    return {res, new WByteArray (std::move (data))};
}