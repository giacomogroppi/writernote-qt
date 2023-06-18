#include "FileReader.h"

FileReader::FileReader()
        : _data(nullptr)
{
}

FileReader::FileReader(const SharedPtr<const WByteArray> &data)
        : _data(data)
{
}

FileReader::FileReader(SharedPtr<const WByteArray> &&data)
        : _data(std::move(data))
{
}

bool FileReader::isOk() const
{
    return this->_data != nullptr;
}
