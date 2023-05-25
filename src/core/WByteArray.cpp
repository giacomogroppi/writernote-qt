#include "WByteArray.h"
#include "utils/WCommonScript.h"

WByteArray::WByteArray()
    : _data(nullptr)
    , _size(0)
    , _reserved(0)
{
}

WByteArray::WByteArray(const char *data, int size)
{
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


