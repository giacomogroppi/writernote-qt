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

void FileWriter::write(const void *data, size_t size)
{
    if (_data.capacity() < size)
        _data.reserve(FileWriter::dataReservePerTime + size);

    this->_data.append(static_cast<const char *>(data), size);
}

void FileWriter::destroy() noexcept
{
    _data = WByteArray();
    _name = WString();
}

FileWriter::FileWriter(FileWriter &&other) noexcept
    : _name(std::move(other._name))
    , _data(std::move(other._data))
{
}
