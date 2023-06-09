//
// Created by Giacomo Groppi on 07/06/23.
//

#ifndef TESTING_QML_FILEREADER_H
#define TESTING_QML_FILEREADER_H

#include "WByteArray.h"
#include "FileInstance.h"
#include <memory>

class FileReader: public FileInstance {
    std::shared_ptr<const WByteArray> _data;
public:
    FileReader (std::shared_ptr<const WByteArray> &data);
    FileReader (std::shared_ptr<const WByteArray> &&data);
    ~FileReader() = default;

    [[nodiscard]]
    bool isOk() const;

    size_t readRaw(void *to, size_t size) const;
};

inline FileReader::FileReader(std::shared_ptr<const WByteArray> &data)
    : _data(data)
{

}

inline FileReader::FileReader(std::shared_ptr<const WByteArray> &&data)
    : _data(std::move(data))
{}

size_t FileReader::readRaw(void *to, size_t size) const
{
    if (size > _data->size()) {
        memcpy(to, _data->constData(), size);
        return size;
    }

    return _data->size();
}

#endif //TESTING_QML_FILEREADER_H
