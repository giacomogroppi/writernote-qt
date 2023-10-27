#include "WZipWriterSingle.h"
#include "WZipWriter.h"

WZipWriterSingle::~WZipWriterSingle()
{
    if(_allocated)
        WFree(this->_data);
}

void WZipWriterSingle::write_string(const char *str, int size)
{
    static_assert(sizeof(size) == 4);
    this->write_object(size);
    if(size)
        this->write(str, size);
}

int WZipWriterSingle::commit_change(const WByteArray &zipName, const WByteArray &nameFileInZip)
{
    WZipWriter writer;

    W_ASSERT(this->_allocated);
    W_ASSERT(this->_data);

    /**
     * Vuol dire che abbiamo impostato una dimensione
     * e dobbiamo rispettarla
    */
    W_ASSERT(this->_max == this->_offset);

    if(writer.init(zipName.constData()) < 0)
        return -1;
    if(writer.write(this->_data, this->_max, nameFileInZip.constData()))
        return -1;
    return 0;
}

auto WZipWriterSingle::write(const void *from, size_t size_object) -> Error
{
    W_ASSERT(this->_offset + size_object <= this->_max);
    W_ASSERT(size_object);
    W_ASSERT(from);

    void *to = this->_data + this->_offset;
    WUtils::WMemcpy(to, from, size_object);
#ifdef DEBUGINFO
    for(size_t i = 0; i < size_object; i++){
        const char *d1 = (const char *) to;
        const char *d2 = (const char *) from;
        W_ASSERT(d1[i] == d2[i]);
    }
#endif
    this->_offset += size_object;

    return Error::makeOk();
}


#ifdef DEBUGINFO
void WZipWriterSingle::set_offset(size_t size)
{
    W_ASSERT(size >= _offset);
    this->_offset = size;
}
#endif
