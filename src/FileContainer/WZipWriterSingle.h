#pragma once

#include "testing/memtest.h"
#include "utils/WCommonScript.h"
#include "core/ByteArray/WByteArray.h"

class WZipWriterSingle {
private:
    /* _data must already be allocated */
    char    *_data;
    size_t  _min, _max;
    size_t  _offset;
    bool    _allocated;

public:
    WZipWriterSingle();
    ~WZipWriterSingle();

    void init(char *_data, size_t min, size_t max);

    void write(const void *from, size_t size_object);

    void *get_data();
    size_t get_offset() const;
    size_t get_size_file() const;

#ifdef DEBUGINFO
    void set_offset(size_t off);
#endif // DEBUGINFO

    template<class T>
    void write_object(const T &data);

    void write_string(const char *str, int size);

    int commit_change(const WByteArray &zipName, const WByteArray &nameFileInZip);

    WZipWriterSingle &operator=(WZipWriterSingle &) = delete;

};

inline size_t WZipWriterSingle::get_size_file() const
{
    W_ASSERT(this->_min == 0);
    return this->_max;
}

inline size_t WZipWriterSingle::get_offset() const
{
    return this->_offset;
}

inline void *WZipWriterSingle::get_data()
{
    W_ASSERT(this->_data);
    return this->_data;
}

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
    this->_allocated = (data == nullptr);

    W_ASSERT(max > min);

    this->_max = max;
    this->_min = min;

    if(_allocated){
        this->_data = (typeof(_data))WMalloc(max);
        W_ASSERT(_max > 0 and _min == 0);
    }else{
        this->_data = data;
    }

    this->_offset = min;
}

template <class T>
inline void WZipWriterSingle::write_object(const T &data)
{
    this->write(&data, sizeof(T));
}
