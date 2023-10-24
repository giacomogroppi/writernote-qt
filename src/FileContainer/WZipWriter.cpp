#include "WZipWriter.h"

WZipWriter::WZipWriter()
{
#ifdef DEBUGINFO
    this->already_init = false;
    this->already_write = false;
    this->_zip = nullptr;
#endif
}

auto WZipWriter::write(const void *to, size_t size, const char *fileToCreate) -> int
{
#ifdef DEBUGINFO
    W_ASSERT(this->already_write == false);
    already_write = true;
#endif // DEBUGINFO

    W_ASSERT(size > 0);
    W_ASSERT(to != nullptr);
    W_ASSERT(fileToCreate != nullptr);
    W_ASSERT(this->_zip != nullptr);

    auto file = FileWriter(fileToCreate);

    if (file.isOk())
        return -1;

    file.write(to, size);

    if (this->_zip->addFile(file)) {
        file.destroy();
        return -1;
    }


    this->close_zip();

    return 0;
}
