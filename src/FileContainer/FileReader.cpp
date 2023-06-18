#include "FileReader.h"

FileReader::FileReader()
        : _data(nullptr)
{
}

FileReader::FileReader(std::shared_ptr<const WByteArray> &data)
        : _data(data)
{
}

FileReader::FileReader(std::shared_ptr<const WByteArray> &&data)
        : _data(std::move(data))
{
}

bool FileReader::isOk() const
{
    return this->_data != nullptr;
}
