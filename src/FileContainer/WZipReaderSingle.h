#pragma once

#include "FileContainer/WZip.h"
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
    [[nodiscard]] int readString(WString &str);
    [[nodiscard]] int readBySize(void *to, size_t size);

    template <class T>
    [[nodiscard]] int readObject(T &object);

    [[nodiscard]] size_t getOffset() const;

    void init(WZip *zip, size_t offset);

    [[nodiscard]] bool isDataAvailable() const;

    WZip *getZip();
};

inline bool WZipReaderSingle::isDataAvailable() const
{
    W_ASSERT(_zip);
    return _zip->is_data_available();
}

inline WZip *WZipReaderSingle::getZip()
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

inline size_t WZipReaderSingle::getOffset() const
{
    return this->offset;
}

inline int WZipReaderSingle::readBySize(void *to, size_t size)
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
inline int WZipReaderSingle::readObject(T &object)
{
    return this->readBySize(&object, sizeof(object));
}

inline int WZipReaderSingle::readString(WString &str)
{
    int l;
    if(this->readObject(l))
        return -1;

    char tmp[l];
    if(l and this->readBySize(tmp, l))
        return -1;
    str = WString::fromUtf8(tmp, l);

    return 0;
}

