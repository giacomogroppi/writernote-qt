#pragma once

#include "utils/common_script.h"
#include <zip.h>

class WZipWriter {
private:

#ifdef DEBUGINFO
    bool already_write;
#endif
    struct zip *_zip;

    static struct zip* openZip(const char *path);

public:
    WZipWriter();
    ~WZipWriter();

    int init(const char *fileZip, const char *fileToWrite);
    int write(const void *to, size_t size);
};

inline int WZipWriter::init(const char *fileZip, const char *fileToWrite)
{
    W_ASSERT(0);
    return -1;
}
