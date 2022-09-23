#pragma once

#include "core/WZip.h"
#include "utils/common_script.h"
#include "testing/memtest.h"

class WZipReaderSingle
{
private:
    WZip *_zip;
    size_t offset;
public:
    WZipReaderSingle();
    WZipReaderSingle(WZip *zip, size_t offset);
    ~WZipReaderSingle() = default;

    const void* read(size_t size);

    int read_by_size(void *to, size_t size);

    template <class T>
    int read_object(T &object);

    int read_string(QString &str);

    size_t get_offset() const;

    void init(WZip *zip, size_t offset);

    void *operator new(size_t number)
    {
        const auto size = sizeof(WZipReaderSingle);
        void *res = WMalloc(size);
        return res;
    }

    WZip *get_zip();

    void operator delete(void *ptr)
    {
        WFree(ptr);
    }
};

inline WZip *WZipReaderSingle::get_zip()
{
    W_ASSERT(_zip);
    return this->_zip;
}

inline void WZipReaderSingle::init(WZip *zip, size_t offset)
{
    W_ASSERT(this->_zip == NULL);
    W_ASSERT(this->offset = 0);
    this->_zip = zip;
    this->offset = offset;
}

inline WZipReaderSingle::WZipReaderSingle()
{
#ifdef DEBUGINFO
    this->_zip = NULL;
    this->offset = 0;
#endif
}

force_inline WZipReaderSingle::WZipReaderSingle(WZip *zip, size_t offset)
{
    this->init(zip, offset);
}

inline const void *WZipReaderSingle::read(size_t size)
{
    if(size + offset >= this->_zip->length()){
        return NULL;
    }
    return _zip->get_data() + size;
}

inline size_t WZipReaderSingle::get_offset() const
{
    return this->offset;
}

inline int WZipReaderSingle::read_by_size(void *to, size_t size)
{
    const auto *data = this->read(size);
    if(!data)
        return 1;

    WMemcpy(to, data, size);
    return 0;
}

template <class T>
inline int WZipReaderSingle::read_object(T &object)
{
    return this->read_by_size(&object, sizeof(object));
}

inline int WZipReaderSingle::read_string(QString &str)
{
    int l;
    if(this->read_object(l))
        return 1;

    char tmp[l];
    if(l && this->read_by_size(tmp, l))
        return 1;
    str = QString::fromUtf8(tmp, l);

    return 0;
}

