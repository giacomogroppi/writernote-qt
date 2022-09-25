#include "WReadZip.h"
#include "WZip.h"
#include "testing/memtest.h"
#include "stdlib.h"
#include <vector>

WReadZip::WReadZip(WZip *zip, int number_thread, const size_t seek, const size_t *base)
{
    W_ASSERT(number_thread > 0);
    W_ASSERT(zip);
    W_ASSERT(base);

    int i;

    this->_zip = zip;

    this->_reader = new WZipReaderSingle[number_thread];

    for(i = 0; i < number_thread; i++){
        this->_reader->init(zip, base[i] + seek);
    }

#ifdef DEBUGINFO
    this->thread_created = number_thread;
#endif //DEBUGINFO

}

WReadZip::~WReadZip()
{
    W_ASSERT(this->_reader);
    W_ASSERT(this->_zip);

    delete [] this->_reader;
    this->_reader = nullptr;
}

const void *WReadZip::read(size_t sizeData, int identifier)
{
    W_ASSERT(identifier < this->thread_created);
    return _reader[identifier].read(sizeData);
}
