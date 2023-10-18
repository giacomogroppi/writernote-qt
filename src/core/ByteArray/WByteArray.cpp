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
        ReadableAbstract &readable) -> WPair<Error, WByteArray>
{
    WByteArray result;

    if (versionController.getVersionWByteArray() != 0)
        return {Error::makeErrVersion(), {}};

    TypeofSize size;
    if (auto err = readable.read(size))
        return {err, {}};

    char d[size];
    if (auto err = readable.read(d, size))
        return {err, {}};

    result = WByteArray(d, size);

    return {Error::makeOk(), result};
}

auto WByteArray::write(WritableAbstract &writable, const WByteArray &object) -> Error
{
    TypeofSize size = object.size();

    if (auto err = writable.write(size))
        return err;

    if (auto err = writable.write(object.constData(), size))
        return err;

    return Error::makeOk();
}

auto WByteArray::loadPtr(
        const VersionFileController &versionFile,
        ReadableAbstract &readableAbstract
) -> WPair<Error, WByteArray *>
{
    auto [res, data] = WByteArray::load (versionFile, readableAbstract);
    if (res)
        return {res, nullptr};

    return {Error::makeOk(), new WByteArray (std::move (data))};
}