#pragma once

#include "utils/WCommonScript.h"
#include "core/FileContainer.h"
#include "core/FileWriter.h"

class WZipWriter {
private:

#ifdef DEBUGINFO
    bool already_write;
    bool already_init;
#endif
    FileContainer          *_zip;

    static FileContainer* openZip(const char *path);
    static int add_file(FileContainer *fileZip, const char *fileName, FileWriter *file);
    static int commit_change(FileWriter *file_change);
    static void destroy_file(struct zip_source *file);

    void close_zip();

public:
    WZipWriter();

    ~WZipWriter();

    int init(const char *file);
    int write(const void *to, size_t size, const char *fileToWrite);
};

inline void WZipWriter::close_zip()
{
    W_ASSERT(this->_zip);
    _zip->close();
    this->_zip = nullptr;
}

inline WZipWriter::~WZipWriter()
{
    if(this->_zip)
        this->close_zip();
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
