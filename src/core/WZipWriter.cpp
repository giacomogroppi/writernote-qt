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

    auto file = FileWriter(fileToCreate);

    if(un(file.isOk()))
        return -1;

    const auto res = file.write(to, size);
    if(res != static_cast<int64_t>(size)){
        file.destroy();
        return -1;
    }

    if (this->_zip->addFile(file) < 0) {
        file.destroy();
        return -1;
    }


    this->close_zip();

    return 0;
}
