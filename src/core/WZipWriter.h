#pragma once

#include "utils/common_script.h"
#include <zip.h>

class WZipWriter {
private:

#ifdef DEBUGINFO
    bool already_write;
    bool already_init;
#endif
    struct zip          *_zip;

    static struct zip* openZip(const char *path);
    static int add_file(zip_t *fileZip, const char *fileName, zip_source_t *file);
    static int commit_change(zip_source_t *file_change);
    static void destroy_file(struct zip_source *file);

public:
    WZipWriter();

    ~WZipWriter();

    int init(const char *file);
    int write(const void *to, size_t size, const char *fileToWrite);
};

inline WZipWriter::~WZipWriter()
{
    if(this->_zip)
        zip_close(this->_zip);
}

inline int WZipWriter::init(const char *fileZip)
{
#ifdef DEBUGINFO
    W_ASSERT(this->already_init == false);
    this->already_init = true;
#endif

    this->_zip = openZip(fileZip);
    if(!_zip){
        DO_IF_DEBUG(this->already_init = false);
        return -1;
    }
    return 0;
}
