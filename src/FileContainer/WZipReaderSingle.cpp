#include "WZipReaderSingle.h"

void WZipReaderSingle::init(WZip *zip, size_t offset)
{
    W_ASSERT(this->_zip == nullptr);
    W_ASSERT(this->offset == 0);
    this->_zip = zip;
    this->offset = offset;
}

WZipReaderSingle::WZipReaderSingle():
    _zip(nullptr),
    offset(0)
{
#ifdef DEBUGINFO
    this->_zip = NULL;
    this->offset = 0;
#endif
}
