#include "WZipWriter.h"

WZipWriter::WZipWriter()
{
#ifdef DEBUGINFO
    this->already_init = false;
    this->already_write = false;
    this->_zip = nullptr;
#endif
}

int WZipWriter::write(const void *to, size_t size, const char *fileToCreate)
{
#ifdef DEBUGINFO
    W_ASSERT(this->already_write == false);
    already_write = true;
#endif // DEBUGINFO

    W_ASSERT(size > 0);
    W_ASSERT(to != nullptr);
    W_ASSERT(fileToCreate != nullptr);
    W_ASSERT(this->_zip != nullptr);

    struct zip_source *file = zip_source_buffer_create(0, 0, 0, NULL);

    if(un(!file))
        return -1;

    zip_source_begin_write(file);

    const auto res = zip_source_write(file, to, size);
    if(res < 0 or res != static_cast<int64_t>(size)){
        destroy_file(file);
        return -1;
    }

    if(commit_change(file) < 0){
        destroy_file(file);
        return -1;
    }

    if(add_file(this->_zip, fileToCreate, file) < 0){
        destroy_file(file);
        return -1;
    }

    this->close_zip();

    return 0;
}

struct zip* WZipWriter::openZip(const char *path)
{
    const int flag = ZIP_CREATE;
    return zip_open(path, flag, nullptr);
}

/* return 0 on success */
inline int WZipWriter::commit_change(zip_source_t *file_change)
{
    /*
     * Upon successful completion 0 is returned. Otherwise, -1 is returned
     * and the error information in source is set to indicate the error.
    */
    const auto res = zip_source_commit_write(file_change);
    if(res == -1)
        return -1;
    return 0;
}

void WZipWriter::destroy_file(zip_source *file)
{
    zip_source_free(file);
}

// return true when success
inline int WZipWriter::add_file(zip_t *fileZip, const char *fileName, zip_source_t *file)
{
    /*
     * Upon successful completion, zip_file_add() returns the index of
     * the new file in the archive, and zip_file_replace() returns 0.
     * Otherwise, -1 is returned and the error code in archive is set
     * to indicate the error.
    */
    const auto res = zip_file_add(fileZip, fileName, file, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);

    if(res == -1)
        return -1;
    return 0;
}
