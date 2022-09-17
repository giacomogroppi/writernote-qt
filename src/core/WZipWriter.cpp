#include "WZipWriter.h"

WZipWriter::WZipWriter()
{

}

int WZipWriter::write(const void *to, size_t size, const char *fileToCreate)
{
#ifdef DEBUGINFO
    W_ASSERT(this->already_write == false);
    already_write = true;
#endif // DEBUGINFO

    struct zip_source *file = zip_source_buffer_create(0, 0, 0, NULL);
    zip_source_begin_write(file);

    if(zip_source_write(file, to, size) < 0){
        destroy_file(file);
        return -1;
    }

    if(commit_change(file)){
        destroy_file(file);
        return -1;
    }

    if(add_file(this->_zip, fileToCreate, file)){
        destroy_file(file);
        return -1;
    }

    zip_close(this->_zip);

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
    return (res == -1) ? -1 : 0;
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

    return (res == -1) ? -1 : 0;
}
