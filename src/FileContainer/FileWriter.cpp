#include "FileWriter.h"

#include <utility>

FileWriter::FileWriter(WString nameFile)
    : _data()
    , _name(std::move(nameFile))
{
    _data.reserve(FileWriter::dataReservePerTime);
}

bool FileWriter::isOk() const
{
    return true;
}

auto FileWriter::write(const void *data, size_t size) -> Error
{
    if (_data.capacity() < size)
        _data.reserve(FileWriter::dataReservePerTime + size);

    this->_data.append(static_cast<const char *>(data), size);
    return Error::makeOk();
}

void FileWriter::destroy() noexcept
{
    _data = WByteArray();
    _name = WString();
}

FileWriter::FileWriter(FileWriter &&other) noexcept
    : _data(std::move(other._data))
    , _name(std::move(other._name))
{
}

auto FileWriter::getName() const noexcept -> const WString &
{
    return _name;
}

auto FileWriter::getData() const noexcept -> const WByteArray &
{
    return _data;
}
