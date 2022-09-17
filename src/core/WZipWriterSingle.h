#pragma once

#include "iostream"
#include "utils/common_script.h"

class WZipWriterSingle {
private:
    /* _data must already be allocated */
    char *_data;
    size_t _min, _max;
    size_t _offset;

public:
    WZipWriterSingle();
    ~WZipWriterSingle() = default;

    void init(char *_data, size_t min, size_t max);

    int write(const void *to, size_t size_object);

    template<class T>
    int write_object(const T &data);

    WZipWriterSingle &operator=(WZipWriterSingle &) = delete;

};

inline WZipWriterSingle::WZipWriterSingle()
{
    this->_data = nullptr;
    this->_min = 1;
    this->_max = 0;
    this->_offset = this->_min;
}

inline void WZipWriterSingle::init(char *data, size_t min, size_t max)
{
    W_ASSERT(_data == nullptr and _min == 1 and _max == 0);
    this->_data = data;
    this->_max = max;
    this->_min = min;
    this->_offset = min;
}

inline int WZipWriterSingle::write(const void *to, size_t size_object)
{
    W_ASSERT(this->_offset < this->_max);

    if(this->_offset + size_object >= this->_max)
        return -1;

    WMemcpy(this->_data + this->_offset, to, size_object);
    this->_offset += size_object;

    return 0;
}

template <class T>
inline int WZipWriterSingle::write_object(const T &data)
{
    return this->write(&data, sizeof(T));
}