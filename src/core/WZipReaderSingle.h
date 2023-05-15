#pragma once

#include "core/WZip.h"
#include "utils/WCommonScript.h"

class WZipReaderSingle
{
private:
    WZip *_zip;
    size_t offset;
public:
    WZipReaderSingle();
    WZipReaderSingle(WZip *zip, size_t offset);
    ~WZipReaderSingle() = default;

    [[nodiscard]] const void* read(size_t size);
    [[nodiscard]] int read_string(WString &str);
    [[nodiscard]] int read_by_size(void *to, size_t size);

    template <class T>
    [[nodiscard]] int read_object(T &object);

    [[nodiscard]] size_t get_offset() const;

    void init(WZip *zip, size_t offset);

    [[nodiscard]] bool is_data_available() const;

    WZip *get_zip();
};

inline bool WZipReaderSingle::is_data_available() const
{
    W_ASSERT(_zip);
    return _zip->is_data_available();
}

inline WZip *WZipReaderSingle::get_zip()
{
    W_ASSERT(_zip);
    return this->_zip;
}

force_inline WZipReaderSingle::WZipReaderSingle(WZip *zip, size_t offset):
    _zip(nullptr),
    offset(0)
{
    this->init(zip, offset);
}

inline const void *WZipReaderSingle::read(size_t size)
{
    if(!(size + offset <= this->_zip->length())){
        return NULL;
    }

    const auto *ret = _zip->get_data() + offset;
    this->offset += size;

    return ret;
}

inline size_t WZipReaderSingle::get_offset() const
{
    return this->offset;
}

inline int WZipReaderSingle::read_by_size(void *to, size_t size)
{
    DO_IF_DEBUG(
    const auto off = this->offset;
                );

    const auto *data = this->read(size);

    W_ASSERT(size > 0);

    if(!data){
        W_ASSERT(this->offset == off);
        return -1;
    }

    W_ASSERT(this->offset == off + size);

    WCommonScript::WMemcpy(to, data, size);

    return 0;
}

template <class T>
inline int WZipReaderSingle::read_object(T &object)
{
    return this->read_by_size(&object, sizeof(object));
}

inline int WZipReaderSingle::read_string(WString &str)
{
    int l;
    if(this->read_object(l))
        return -1;

    char tmp[l];
    if(l and this->read_by_size(tmp, l))
        return -1;
    str = WString::fromUtf8(tmp, l);

    return 0;
}

