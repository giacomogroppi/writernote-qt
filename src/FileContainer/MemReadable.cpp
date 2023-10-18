#include "MemReadable.h"
#include "utils/WCommonScript.h"

MemReadable &MemReadable::setData(const void *data, size_t max)
{
    this->_data = static_cast<const char *>(data);
    this->_max = max;
    this->_seek = 0u;
    return *this;
}

auto MemReadable::read(void *to, size_t size) const -> Error
{
    W_ASSERT(to != nullptr and size >= 0);

    if (!size)
        return Error::makeOk();

    if (size + this->_seek > _max) {
        W_ASSERT(0);
        return Error::makeCorruption();
    }

    WCommonScript::WMemcpy(to, _data + _seek, size);
    _seek += size;

    return Error::makeOk();
}

MemReadable::MemReadable()
    : _data(nullptr)
    , _seek(0)
    , _max(0)
{

}

MemReadable::MemReadable(const void *data, size_t max)
     : _data(static_cast<const char *>(data))
     , _seek(0)
     , _max(max)
{

}

bool MemReadable::isAtEnd() const noexcept
{
    return this->_seek == _max;
}

auto MemReadable::getSeek() const noexcept -> size_t
{
    return this->_seek;
}

auto MemReadable::getMax() const noexcept -> size_t
{
    return _max;
}
