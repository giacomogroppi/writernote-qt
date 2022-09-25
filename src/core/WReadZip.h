#pragma once

#include "WZip.h"
#include "stdio.h"
#include "utils/common_script.h"
#include "core/WZipReaderSingle.h"
#include <vector>

class WReadZip{
private:
    WZip    *_zip;
    WZipReaderSingle* _reader;

#ifdef DEBUGINFO
    int thread_created;
#endif

public:
    // pass readDataFrom != 0 if you want to seek to some position
    WReadZip(WZip *zip, int number_thread, const size_t readDataFrom, const size_t *base);
    ~WReadZip();

    WZipReaderSingle *get_reader(int id);

    const void *read(size_t size, int identifier);
    [[nodiscard]] size_t get_current_distance(int index_thread) const;
};

force_inline WZipReaderSingle *WReadZip::get_reader(int id)
{
    W_ASSERT(id < this->thread_created);
    return &_reader[id];
}

force_inline size_t WReadZip::get_current_distance(int index_thread) const
{
    W_ASSERT(thread_created < index_thread);
    return _reader[index_thread].get_offset();
}
